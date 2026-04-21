#ifndef FIX_Modules_Session_dot_h
#define FIX_Modules_Session_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"

#include "Common/Config.h"

#include "FIX/Types.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol Session. Validates incoming messages and handles login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Session
{
public:
    using State = Context::Session::State;

    using Logon         = Factory::Logon;
    using Logout        = Factory::Logout;
    using ResendRequest = Factory::ResendRequest;

    template<typename Context>
    Session(const Config& config, Context& context)
      : m_context(context)
      , m_factory(context)
    {
        m_context.state(State::LogonWait);
    }

    template<typename Message, typename Next>
    bool put(const Message& msg, Next& next)
    {
        log << level::debug << vmark << '[' << msg << ']' << std::endl;

        validate(msg);

        auto msgtype = msg.msgtype();
        if (msgtype == Logon::type()) {
            return logon(msg, next);
        }

        m_context.iAdvance();
        log << level::debug << ts << here << ' ' << m_context.iSequence() << ':' << msg[tag::MsgSeqNum] << std::endl;

        // TODO - Check incoming sequence number

        if (msgtype == Logout::type()) {
            return logout(msg, next);
        }

        if (msgtype == ResendRequest::type()) {
            return resend(msg, next);
        }
        return true;
    }

private:
    template<typename Message>
    void validate(const Message& msg)
    {
        auto begstr = msg.prefix();
        if (begstr != m_context.BeginString) {
            throw std::runtime_error("Invalid Begin String. Expecting [" + m_context.BeginString + "] got [" + std::string(begstr) + ']');
        }

        auto scompid = msg[tag::SenderCompID];
        if (scompid != m_context.TargetCompID) {
            throw std::runtime_error("Invalid Sender Comp ID. Expecting [" + m_context.TargetCompID + "] got [" + std::string(scompid) + ']');
        }

        auto tcompid = msg[tag::TargetCompID];
        if (tcompid != m_context.SenderCompID) {
            throw std::runtime_error("Invalid Target Comp ID. Expecting [" + m_context.SenderCompID + "] got [" + std::string(tcompid) + ']');
        }

        auto msgtype = msg.msgtype();
        if (msgtype.empty()) {
            throw std::runtime_error("Missing Message Type.");
        }

        auto msequence = msg[tag::MsgSeqNum];
        if (msequence.empty()) {
            throw std::runtime_error("Missing Sequence Number.");
        }

        auto mstime = msg[tag::SendingTime];
        if (mstime.empty()) {
            throw std::runtime_error("Missing Sending Time.");
        }

        if (auto state = m_context.state(); (state == State::LogonSent || state == State::LogonWait) && msgtype != msg_type::Logon) {
            throw std::runtime_error("Logon must be the first message.");
        }
    }

    template<typename Message, typename Next>
    bool logon(const Message& msg, Next& next)
    {
        if (auto state = m_context.state(); (state != State::LogonSent && state != State::LogonWait)) {
            std::string error = "Unexpected Logon message at this time. " + to_string(m_context.state());
            next.put(m_factory.logout(error));
            return false;
        }

        auto encrypt = msg[tag::EncryptMethod];
        if (auto value = to_int<int>(encrypt); value != 0) {
            std::string error = "Invalid encryption method. Expecting [0] got [" + std::string(encrypt) + ']';
            next.put(m_factory.logout(error));
            return false;
        }

        auto heartbtint = msg[tag::HeartBtInt];
        if (auto value = to_int<int>(heartbtint); value == 0) {
            std::string error = "Invalid heartbit interval [" + std::string(heartbtint) + ']';
            next.put(m_factory.logout(error));
            return false;
        } else {
            m_context.HBInterval = value;
        }

        if (msg.has(tag::ResetSeqNumFlag)) {
            if (msg[tag::ResetSeqNumFlag] == "Y") {
                m_context.reset();
            }
        }

        if (m_context.state() == State::LogonWait) {
            next.put(m_factory.logon());
        }

        m_context.state(State::Normal);
        return false;
    }

    template<typename Message, typename Next>
    bool logout(const Message& msg, Next& next)
    {
        next.put(m_factory.logout());
        m_context.state(State::LogonWait);
        return false;
    }

    template<typename Message, typename Next>
    bool resend(const Message& msg, Next& next)
    {
        next.put(m_factory.gapfill());
        return false;
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace FIX::Modules
#endif
