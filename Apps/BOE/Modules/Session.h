#ifndef BOE_Modules_Session_dot_h
#define BOE_Modules_Session_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "BOE/Context/Session.h"

#include "Common/Config.h"

namespace boe::Modules {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// BOE protocol Session. Validates incoming messages and handles login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Session
{
    using Config = Common::Config;
    using State  = Context::Session::State;

    using MessageHeader = typename Factory::MessageHeader;
    using LoginRequest  = typename Factory::LoginRequest;
    using LogoutRequest = typename Factory::LogoutRequest;
    using LoginResponse = typename Factory::LoginResponse;

public:
    template<typename Context>
    explicit Session(Context& context)
      : m_session(context)
      , m_factory(context)
    {
        m_session.state(State::LogonWait);
    }

    template<typename Context>
    Session(const Config&, Context& context)
      : m_session(context)
      , m_factory(context)
    {
        m_session.state(State::LogonWait);
    }

    template<typename Message, typename Next>
    bool put(const Message& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        validate(msg);

        if (msg.messageType == LoginRequest::Type) {
            return process(reinterpret_cast<const LoginRequest&>(msg), next);
        }
        if (msg.messageType == LoginResponse::Type) {
            return process(reinterpret_cast<const LoginResponse&>(msg), next);
        }
        if (msg.messageType == LogoutRequest::Type) {
            return process(reinterpret_cast<const LogoutRequest&>(msg), next);
        }
        return true;
    }

private:
    void validate(const MessageHeader& msg)
    {
        log << level::trace << ts << here << std::endl;
        if (auto state = m_session.state(); state == State::LogonSent && msg.messageType != LoginResponse::Type) {
            throw std::runtime_error("Logon Response must be the first message.");
        }
        if (auto state = m_session.state(); state == State::LogonWait && msg.messageType != LoginRequest::Type) {
            throw std::runtime_error("Logon Request must be the first message.");
        }
    }

    template<typename Next>
    bool process(const LoginRequest& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        if (auto state = m_session.state(); state != State::LogonWait) {
            throw std::runtime_error("Unexpected Logon Request message at this time. " + to_string(m_session.state()));
        }
        if (msg.username != m_session.UserName) {
            throw std::runtime_error("Invalid user name. Expecting: " + m_session.UserName + ", got: " + std::string(msg.username));
        }
        if (msg.password != m_session.Password) {
            throw std::runtime_error("Invalid password. Expecting: " + m_session.Password + ", got: " + std::string(msg.password));
        }

        if (m_session.state() == State::LogonWait) {
            next.put(m_factory.loginAccept());
        }

        m_session.state(State::Normal);
        return false;
    }

    template<typename Next>
    bool process(const LoginResponse& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        if (auto state = m_session.state(); state != State::LogonSent) {
            throw std::runtime_error("Unexpected Logon Accepted message at this time. " + to_string(m_session.state()));
        }
        m_session.state(State::Normal);
        return false;
    }

    template<typename Next>
    bool process(const LogoutRequest& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        next.put(m_factory.logoutRequest());
        m_session.state(State::LogonWait);
        throw std::runtime_error("Terminating session after logout.");
    }

private:
    Context::Session& m_session;
    Factory           m_factory;
};

} // namespace boe::Modules
#endif
