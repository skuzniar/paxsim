#ifndef OUCH_Modules_Factory_OUCH50_OrderCancel_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"
#include "OUCH/OUCH50/OrderCancelled.h"

#include "Common/Config.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderCancel
{
    using Config = Common::Config;

    using PacketHeader   = OUCH::OUCH50::PacketHeader;
    using OrderCancelled = OUCH::OUCH50::OrderCancelled;
    using Timestamp      = OUCH::OUCH50::Timestamp;

    template<typename Context>
    explicit OrderCancel(Context& context)
    {
    }

    auto cancel(const Order& order, const Config::Table& params)
    {
        OrderCancelled omsg;

        std::string reason = params["Reason"];

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::template enum_cast<OrderCancelled::CancelReason>(reason);

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
