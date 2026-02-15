#ifndef Modules_Session_Session_dot_h
#define Modules_Session_Session_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context/Session/Session.h"
#include "Utils.h"

#include <quickfix/Message.h>

namespace Fix::Modules::Session {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Session. Validates incoming messages and handles login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Session
{
public:
    template<typename Context>
    explicit Session(Context& context)
      : m_context(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    bool put(const FIX::Message& msg, Next& next)
    {
        validate(msg);

        log << level::debug << vmark << '[' << fixdump(msg.toString()) << ']' << std::endl;

        const auto& msgtype = msg.getHeader().getField(FIX::FIELD::MsgType);

        if (msgtype == FIX::MsgType_Logon) {
            return process(typename Factory::Logon(msg), next);
        }

        m_context.iAdvance();
        log << level::debug << ts << here << ' ' << m_context.iSequence() << ':' << msg.getHeader().getField(FIX::FIELD::MsgSeqNum) << std::endl;

        // TODO - Check incoming sequence number

        if (msgtype == FIX::MsgType_Logout) {
            return process(typename Factory::Logout(msg), next);
        }
        if (msgtype == FIX::MsgType_ResendRequest) {
            return process(typename Factory::ResendRequest(msg), next);
        }
        return true;
    }

private:
    void validate(const FIX::Message& msg)
    {
        const auto& begstr = msg.getHeader().getField(FIX::FIELD::BeginString);
        if (begstr != Factory::BeginString) {
            throw std::runtime_error("Invalid Begin String. Expecting [" + std::string(Factory::BeginString) + "] got [" + begstr + ']');
        }

        const auto& scompid = msg.getHeader().getField(FIX::FIELD::SenderCompID);
        if (scompid != m_context.TargetCompID) {
            throw std::runtime_error("Invalid Sender Comp ID. Expecting [" + m_context.TargetCompID + "] got [" + scompid + ']');
        }

        const auto& tcompid = msg.getHeader().getField(FIX::FIELD::TargetCompID);
        if (tcompid != m_context.SenderCompID) {
            throw std::runtime_error("Invalid Target Comp ID. Expecting [" + m_context.SenderCompID + "] got [" + tcompid + ']');
        }

        const auto& msgtype = msg.getHeader().getField(FIX::FIELD::MsgType);
        if (msgtype.empty()) {
            throw std::runtime_error("Missing Message Type.");
        }

        const auto& msequence = msg.getHeader().getField(FIX::FIELD::MsgSeqNum);
        if (msequence.empty()) {
            throw std::runtime_error("Missing Sequence Number.");
        }

        const auto& mstime = msg.getHeader().getField(FIX::FIELD::SendingTime);
        if (mstime.empty()) {
            throw std::runtime_error("Missing Sending Time.");
        }

        if (m_context.state() == Context::Session::State::Startup && msgtype != FIX::MsgType_Logon) {
            throw std::runtime_error("Logon must be the first message.");
        }
    }

    template<typename Next>
    bool process(const Factory::Logon& msg, Next& next)
    {
        if (m_context.state() != Context::Session::State::Startup) {
            std::string error = "Unexpected Logon message at this time. " + to_string(m_context.state());
            next.put(m_factory.logout(error));
            return false;
        }

        auto encrypt = std::stoi(msg.getField(FIX::FIELD::EncryptMethod));
        if (encrypt != 0) {
            std::string error = "Invalid encryption method. Expecting [0] got [" + std::to_string(encrypt) + ']';
            next.put(m_factory.logout(error));
            return false;
        }

        auto heartbtint = std::stoi(msg.getField(FIX::FIELD::HeartBtInt));
        if (heartbtint == 0) {
            std::string error = "Invalid heartbit interval [" + std::to_string(heartbtint) + ']';
            next.put(m_factory.logout(error));
            return false;
        }
        m_context.HBInterval = heartbtint;

        if (msg.isSetField(FIX::FIELD::ResetSeqNumFlag)) {
            const auto& seqreset = msg.getField(FIX::FIELD::ResetSeqNumFlag);
            if (seqreset == "Y") {
                m_context.reset();
            }
        }

        next.put(m_factory.logon());
        m_context.state(Context::Session::State::Normal);
        return false;
    }

    template<typename Next>
    bool process(const Factory::Logout&, Next& next)
    {
        next.put(m_factory.logout());
        m_context.state(Context::Session::State::Startup);
        return false;
    }

    template<typename Next>
    bool process(const Factory::ResendRequest&, Next& next)
    {
        next.put(m_factory.gapfill());
        return false;
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace Fix::Modules::Session
#endif
