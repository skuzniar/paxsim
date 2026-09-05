#ifndef OUCH_Modules_Factory_OUCH50_Writer_dot_h
#define OUCH_Modules_Factory_OUCH50_Writer_dot_h

#include "OUCH/Context/Session.h"

#include "OUCH/OUCH50/ServerHeartbeat.h"

namespace OUCH::Modules::Factory::OUCH50 {

//---------------------------------------------------------------------------------------------------------------------
// OUCH 5.0 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct Writer
{
    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;

    using ServerHeartbeat = OUCH::OUCH50::ServerHeartbeat;

    template<typename Context>
    explicit Writer(Context& context)
      : m_session(context)
    {
    }

    auto heartbeat()
    {
        return ServerHeartbeat();
    }

    Context::Session& m_session;
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
