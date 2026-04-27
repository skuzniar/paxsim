#ifndef OUCH_Modules_Factory_OUCH50_OrderCancelOnDisconnect_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderCancelOnDisconnect_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"
#include "OUCH/OUCH50/OrderCancelled.h"

#include "Common/Config.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace OUCH::OUCH50;
using namespace PaxSim::Core;
using Common::Config;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderCancelOnDisconnect
{
    using PacketHeader   = PacketHeader;
    using OrderCancelled = OrderCancelled;

    template<typename Context>
    explicit OrderCancelOnDisconnect(Context& context)
    {
    }

    PacketHeader& cancel(const Order& order)
    {
        auto& omsg = *new (m_buff) OrderCancelled;

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::CancelReason::UserRequested;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

private:
    char m_buff[1024];
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
