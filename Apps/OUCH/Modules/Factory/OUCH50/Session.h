#ifndef OUCH_Modules_Factory_OUCH50_Session_dot_h
#define OUCH_Modules_Factory_OUCH50_Session_dot_h

#include "Common/Context/Session.h"

#include "OUCH/OUCH50/ServerHeartbeat.h"
#include "OUCH/OUCH50/LoginRequest.h"
#include "OUCH/OUCH50/LoginAccepted.h"
#include "OUCH/OUCH50/LogoutRequest.h"
#include "OUCH/OUCH50/AccountQuery.h"
#include "OUCH/OUCH50/AccountQueryResponse.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace OUCH::OUCH50;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct Session
{
    using PacketHeader    = PacketHeader;
    using SequencedData   = SequencedData;
    using UnsequencedData = UnsequencedData;

    using ServerHeartbeat      = ServerHeartbeat;
    using LoginRequest         = LoginRequest;
    using LoginAccepted        = LoginAccepted;
    using LogoutRequest        = LogoutRequest;
    using AccountQuery         = AccountQuery;
    using AccountQueryResponse = AccountQueryResponse;

    template<typename Context>
    explicit Session(Context& context)
      : m_session(context)
    {
    }

    PacketHeader& heartbeat()
    {
        return *reinterpret_cast<PacketHeader*>(new (m_buff) ServerHeartbeat);
    }

    PacketHeader& loginRequest(std::string_view username, std::string_view password)
    {
        auto* request = new (m_buff) LoginRequest;

        request->userName = username;
        request->password = password;

        return *reinterpret_cast<PacketHeader*>(request);
    }

    PacketHeader& loginAccept()
    {
        return *reinterpret_cast<PacketHeader*>(new (m_buff) LoginAccepted);
    }

    PacketHeader& logoutRequest()
    {
        return *reinterpret_cast<PacketHeader*>(new (m_buff) LogoutRequest);
    }

    PacketHeader& accountQuery()
    {
        return *reinterpret_cast<PacketHeader*>(new (m_buff) AccountQuery);
    }

    PacketHeader& accountQueryResponse()
    {
        auto* response = new (m_buff) AccountQueryResponse;

        response->timestamp = Timestamp::now();
        response->nextUserRefNum = m_session.iSequence();

        return *reinterpret_cast<PacketHeader*>(response);
    }

private:
    Common::Context::Session& m_session;
    char                      m_buff[1024];
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
