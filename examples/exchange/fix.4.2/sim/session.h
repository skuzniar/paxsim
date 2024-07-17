#ifndef Paxsim_Examples_Fix42_Sim_Session_dot_h
#define Paxsim_Examples_Fix42_Sim_Session_dot_h

#include "quickfix/fix42/Heartbeat.h"
#include "quickfix/fix42/ResendRequest.h"
#include "quickfix/fix42/SequenceReset.h"
#include "quickfix/fix42/Logon.h"
#include "quickfix/fix42/Logout.h"

#include "quickfix/Message.h"
#include "core/streamlog.h"
#include "types.h"

#include <nlohmann/json.hpp>
#include <vector>
#include <optional>

namespace fix42::sim {

using namespace paxsim::core;
using paxsim::core::log;

using json = nlohmann::json;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Context.
//---------------------------------------------------------------------------------------------------------------------
struct SessionContext
{
    SessionContext(const json& cfg)
      : SenderCompID(cfg["Session"]["SenderCompID"])
      , TargetCompID(cfg["Session"]["TargetCompID"])
    {
    }

    // Sender and target comp IDs
    std::string SenderCompID = "SERVER";
    std::string TargetCompID = "CLIENT";

    // Incoming and outgoing sequence numbers
    std::size_t ISequence = 1;
    std::size_t OSequence = 1;

    // Heartbeat interval
    std::size_t HBInterval = 30;
};

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Session. We ensure that the incommins message fulfills session requirements.
//---------------------------------------------------------------------------------------------------------------------
class Session
{
    static constexpr const char* iam = "Session   ";

public:
    Session(SessionContext& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<FIX::Message>> process(FIX::Message message)
    {
        // If a message can not be validated we drop the session
        if (!validate(message)) {
            return { {} };
        }

        const auto& msgtype = message.getHeader().getField(FIX::FIELD::MsgType);

        // Sequence number checks
        if (!message.getHeader().isSetField(FIX::FIELD::MsgSeqNum)) {
            std::string error = "Missing sequence number.";
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << error << std::endl;
            return { logout(error) };
        }
        auto sequence = std::stoul(message.getHeader().getField(FIX::FIELD::MsgSeqNum));
        if (sequence < m_context.ISequence && msgtype != FIX::MsgType_SequenceReset) {
            std::string error = "Invalid incoming sequence number. Expecting [" + std::to_string(m_context.ISequence) +
                                "] got [" + std::to_string(sequence) + ']';
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << error << std::endl;
            return { logout(error) };
        }

        // Invalid encryption results in logout
        if (msgtype == FIX::MsgType_Logon) {
            auto encrypt = std::stoul(message.getField(FIX::FIELD::EncryptMethod));
            if (encrypt != 0) {
                std::string error = "Invalid encryption method. Expecting [0] got [" + std::to_string(encrypt) + ']';
                log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << error
                    << std::endl;
                return { logout(error) };
            }
        }

        // TODO - Resend requests generate sequence reset
        if (msgtype == FIX::MsgType_ResendRequest) {
            FIX42::SequenceReset reset;
            reset.getHeader().set(FIX::SenderCompID(m_context.SenderCompID));
            reset.getHeader().set(FIX::TargetCompID(m_context.TargetCompID));
            reset.getHeader().set(FIX::MsgSeqNum(m_context.OSequence++));
            reset.getHeader().set(FIX::SendingTime::now());
            reset.set(FIX::GapFillFlag(false));
            reset.set(FIX::NewSeqNo(m_context.OSequence));

            log << level::info << vmark << iam << '[' << fixdump(reset.toString()) << ']' << std::endl;
            return { { reset } };
        }

        // Close connection on Logout
        if (msgtype == FIX::MsgType_Logout) {
            return { logout(), {} };
        }

        if (msgtype == FIX::MsgType_SequenceReset) {
            m_context.ISequence = std::stoul(message.getField(FIX::FIELD::NewSeqNo));

            // TODO - understand gap fill
            const auto& gapfill = message.getField(FIX::FIELD::GapFillFlag);
            return {};
        }

        if (sequence > m_context.ISequence) {
            m_state = State::Recovery;
            log << level::info << vmark << iam << '[' << fixdump(message.toString()) << ']' << std::endl;
            return { logon(), resend(m_context.ISequence, 0) };
        }

        ++m_context.ISequence;

        m_state = State::Normal;

        if (msgtype == FIX::MsgType_Logon) {
            // Authenticate Logon request here and send logout if failed
            if (authenticate(message)) {
                m_context.HBInterval = std::stoul(message.getField(FIX::FIELD::HeartBtInt));
                return { logon() };
            }
            return { logout("Authentication failed.") };
        }

        // Respond to Test Request
        if (msgtype == FIX::MsgType_TestRequest) {
            return { heartbeat(message.getField(FIX::FIELD::TestReqID)) };
        }

        // Respond to Heartbeat. Not a correct implementation but will do in a pinch
        if (msgtype == FIX::MsgType_Heartbeat) {
            return { heartbeat() };
        }

        // Ignore rejefct messages
        if (msgtype == FIX::MsgType_Reject) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "Ignoring."
                << std::endl;
            return {};
        }

        log << level::debug << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "Passing through."
            << std::endl;
        return { message };
    }

private:
    bool validate(const FIX::Message& message)
    {
        // Preliminary checks
        const auto& begstr = message.getHeader().getField(FIX::FIELD::BeginString);
        if (begstr != FIX::BeginString_FIX42) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Invalid Begin String. Expecting [" << FIX::BeginString_FIX42 << "] got [" << begstr << ']'
                << std::endl;
            return false;
        }

        const auto& scompid = message.getHeader().getField(FIX::FIELD::SenderCompID);
        if (scompid != m_context.TargetCompID) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Invalid Sender Comp ID. Expecting [" << m_context.TargetCompID << "] got [" << scompid << ']'
                << std::endl;
            return false;
        }

        const auto& tcompid = message.getHeader().getField(FIX::FIELD::TargetCompID);
        if (tcompid != m_context.SenderCompID) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Invalid Target Comp ID. Expecting [" << m_context.SenderCompID << "] got [" << tcompid << ']'
                << std::endl;
            return false;
        }

        const auto& msgtype = message.getHeader().getField(FIX::FIELD::MsgType);
        if (msgtype.empty()) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Missing Message Type." << std::endl;
            return false;
        }

        if (m_state == State::Startup && msgtype != FIX::MsgType_Logon) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Logon must be the first message." << std::endl;
            return false;
        }

        // TODO - look at ResetSeqNumFlag

        return true;
    }

    // Authenticate logon request
    bool authenticate(const FIX::Message& message)
    {
        return true;
    }

    FIX42::Logon logon()
    {
        FIX42::Logon logon;
        logon.getHeader().set(FIX::SenderCompID(m_context.SenderCompID));
        logon.getHeader().set(FIX::TargetCompID(m_context.TargetCompID));
        logon.getHeader().set(FIX::MsgSeqNum(m_context.OSequence++));
        logon.getHeader().set(FIX::SendingTime::now());
        logon.set(FIX::HeartBtInt(m_context.HBInterval));
        logon.set(FIX::EncryptMethod(0));

        log << level::info << vmark << iam << '[' << fixdump(logon.toString()) << ']' << std::endl;
        return logon;
    }

    FIX42::Logout logout(const std::string& text = "")
    {
        FIX42::Logout logout;
        logout.getHeader().set(FIX::SenderCompID(m_context.SenderCompID));
        logout.getHeader().set(FIX::TargetCompID(m_context.TargetCompID));
        logout.getHeader().set(FIX::MsgSeqNum(m_context.OSequence++));
        logout.getHeader().set(FIX::SendingTime::now());

        if (!text.empty()) {
            logout.set(FIX::Text(text));
        }

        log << level::info << vmark << iam << '[' << fixdump(logout.toString()) << ']' << std::endl;
        return logout;
    }

    FIX42::Heartbeat heartbeat(const std::string& testreqid = "")
    {
        FIX42::Heartbeat heartbeat;
        heartbeat.getHeader().set(FIX::SenderCompID(m_context.SenderCompID));
        heartbeat.getHeader().set(FIX::TargetCompID(m_context.TargetCompID));
        heartbeat.getHeader().set(FIX::MsgSeqNum(m_context.OSequence++));
        heartbeat.getHeader().set(FIX::SendingTime::now());

        if (!testreqid.empty()) {
            heartbeat.set(FIX::TestReqID(testreqid));
        }

        log << level::info << vmark << iam << '[' << fixdump(heartbeat.toString()) << ']' << std::endl;
        return { heartbeat };
    }

    FIX42::Heartbeat resend(std::size_t begseq, std::size_t endseq)
    {
        FIX42::ResendRequest resend;
        resend.getHeader().set(FIX::SenderCompID(m_context.SenderCompID));
        resend.getHeader().set(FIX::TargetCompID(m_context.TargetCompID));
        resend.getHeader().set(FIX::MsgSeqNum(m_context.OSequence++));
        resend.getHeader().set(FIX::SendingTime::now());
        resend.set(FIX::BeginSeqNo(begseq));
        resend.set(FIX::EndSeqNo(endseq));

        log << level::info << vmark << iam << '[' << fixdump(resend.toString()) << ']' << std::endl;
        return { resend };
    }

private:
    // Trivial state machine to deal with message handing on startup, normal operation and recovery
    enum class State
    {
        Startup,
        Normal,
        Recovery
    };

    State m_state = State::Startup;

    SessionContext& m_context;
};

} // namespace fix42::sim
#endif
