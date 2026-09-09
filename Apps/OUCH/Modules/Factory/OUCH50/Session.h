#ifndef OUCH_Modules_Factory_OUCH50_Session_dot_h
#define OUCH_Modules_Factory_OUCH50_Session_dot_h

#include "OUCH/Context/Session.h"

#include "OUCH/OUCH50/ServerHeartbeat.h"
#include "OUCH/OUCH50/LoginRequest.h"
#include "OUCH/OUCH50/LoginAccepted.h"
#include "OUCH/OUCH50/LogoutRequest.h"
#include "OUCH/OUCH50/AccountQuery.h"
#include "OUCH/OUCH50/AccountQueryResponse.h"

namespace OUCH::Modules::Factory::OUCH50 {

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct Session
{
    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;

    using ServerHeartbeat      = OUCH::OUCH50::ServerHeartbeat;
    using LoginRequest         = OUCH::OUCH50::LoginRequest;
    using LoginAccepted        = OUCH::OUCH50::LoginAccepted;
    using LogoutRequest        = OUCH::OUCH50::LogoutRequest;
    using AccountQuery         = OUCH::OUCH50::AccountQuery;
    using AccountQueryResponse = OUCH::OUCH50::AccountQueryResponse;
    using Timestamp            = OUCH::OUCH50::Timestamp;

    template<typename Context>
    explicit Session(Context& context)
      : m_session(context)
    {
    }

    static auto heartbeat()
    {
        return ServerHeartbeat();
    }

    static auto loginRequest(std::string_view username, std::string_view password)
    {
        return LoginRequest(username, password);
    }

    static auto loginAccept()
    {
        return LoginAccepted();
    }

    static auto logoutRequest()
    {
        return LogoutRequest();
    }

    static auto accountQuery()
    {
        return AccountQuery();
    }

    auto accountQueryResponse()
    {
        AccountQueryResponse response;
        response.timestamp      = Timestamp::now();
        response.nextUserRefNum = m_session.iSequence();
        return response;
    }

private:
    Context::Session& m_session;
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
