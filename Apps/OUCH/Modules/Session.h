#ifndef OUCH_Modules_Session_dot_h
#define OUCH_Modules_Session_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/Session.h"

#include "Common/Config.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH protocol Session. Validates incoming messages and handles login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Session
{
public:
    using State = Context::Session::State;

    using SequencedData   = Factory::SequencedData;
    using UnsequencedData = Factory::UnsequencedData;

    using LoginRequest         = Factory::LoginRequest;
    using LogoutRequest        = Factory::LogoutRequest;
    using LoginAccepted        = Factory::LoginAccepted;
    using AccountQuery         = Factory::AccountQuery;
    using AccountQueryResponse = Factory::AccountQueryResponse;

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
        log << level::trace << ts << here << std::endl;

        validate(msg);

        if (msg.type == LoginRequest::Type) {
            return process(reinterpret_cast<const LoginRequest&>(msg), next);
        }
        if (msg.type == LoginAccepted::Type) {
            return process(reinterpret_cast<const LoginAccepted&>(msg), next);
        }
        if (msg.type == LogoutRequest::Type) {
            return process(reinterpret_cast<const LogoutRequest&>(msg), next);
        }
        if (msg.type == UnsequencedData::Type) {
            const auto& header = reinterpret_cast<const UnsequencedData&>(msg);
            if (header.type == AccountQuery::Type) {
                return process(reinterpret_cast<const AccountQuery&>(msg), next);
            }
        }
        if (msg.type == SequencedData::Type) {
            const auto& header = reinterpret_cast<const SequencedData&>(msg);
            if (header.type == AccountQueryResponse::Type) {
                return process(reinterpret_cast<const AccountQueryResponse&>(msg), next);
            }
        }
        return true;
    }

private:
    void validate(const Factory::PacketHeader& msg)
    {
        log << level::trace << ts << here << std::endl;
        if (auto state = m_context.state(); state == State::LogonSent && msg.type != Factory::LoginAccepted::Type) {
            throw std::runtime_error("Logon Accepted must be the first message.");
        }
        if (auto state = m_context.state(); state == State::LogonWait && msg.type != Factory::LoginRequest::Type) {
            throw std::runtime_error("Logon Request must be the first message.");
        }
    }

    template<typename Next>
    bool process(const LoginRequest& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        if (auto state = m_context.state(); state != State::LogonWait) {
            throw std::runtime_error("Unexpected Logon Request message at this time. " + to_string(m_context.state()));
        }
        if (msg.userName != m_context.UserName) {
            throw std::runtime_error("Invalid user name. Expecting: " + m_context.UserName + ", got: " + std::string(msg.userName));
        }
        if (msg.password != m_context.Password) {
            throw std::runtime_error("Invalid password. Expecting: " + m_context.Password + ", got: " + std::string(msg.password));
        }

        if (m_context.state() == State::LogonWait) {
            next.put(m_factory.loginAccept());
        }

        m_context.state(State::Normal);
        return false;
    }

    template<typename Next>
    bool process(const LoginAccepted& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        if (auto state = m_context.state(); state != State::LogonSent) {
            throw std::runtime_error("Unexpected Logon Accepted message at this time. " + to_string(m_context.state()));
        }
        next.put(m_factory.accountQuery());
        m_context.state(State::Normal);
        return false;
    }

    template<typename Next>
    bool process(const LogoutRequest& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        next.put(m_factory.logoutRequest());
        m_context.state(State::LogonWait);
        return false;
    }

    template<typename Next>
    bool process(const AccountQuery& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        next.put(m_factory.accountQueryResponse());
        return false;
    }

    template<typename Next>
    bool process(const AccountQueryResponse& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        return false;
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace OUCH::Modules
#endif
