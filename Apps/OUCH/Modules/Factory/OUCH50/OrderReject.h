#ifndef OUCH_Modules_Factory_OUCH50_OrderReject_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"

#include "OUCH/OUCH50/EnterOrder.h"
#include "OUCH/OUCH50/ReplaceOrder.h"
#include "OUCH/OUCH50/CancelOrder.h"

#include "OUCH/OUCH50/OrderRejected.h"
#include "OUCH/OUCH50/CancelRejected.h"

#include "Common/Config.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace PaxSim::Core;
using Common::Config;
using PaxSim::Core::log;

using namespace OUCH::OUCH50;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderReject
{
    using PacketHeader    = PacketHeader;
    using SequencedData   = SequencedData;
    using UnsequencedData = UnsequencedData;

    using EnterOrder   = EnterOrder;
    using ReplaceOrder = ReplaceOrder;
    using CancelOrder  = CancelOrder;

    using OrderRejected  = OrderRejected;
    using CancelRejected = CancelRejected;

    template<typename Context>
    OrderReject(Context& context)
    {
    }

    static auto clordID(const EnterOrder& msg)
    {
        return msg.userRefNum;
    }

    auto reject(const EnterOrder& msg, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        OrderRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    static auto clordID(const ReplaceOrder& msg)
    {
        return msg.origUserRefNum;
    }

    auto reject(const ReplaceOrder& msg, const Order& order, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        OrderRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.newUserRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    static auto clordID(const CancelOrder& msg)
    {
        return msg.userRefNum;
    }

    auto reject(const CancelOrder& msg, const Order& order, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        CancelRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
