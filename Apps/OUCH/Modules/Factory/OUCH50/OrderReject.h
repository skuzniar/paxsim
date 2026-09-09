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
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderReject
{
    using Config = Common::Config;

    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;

    using EnterOrder     = OUCH::OUCH50::EnterOrder;
    using ReplaceOrder   = OUCH::OUCH50::ReplaceOrder;
    using CancelOrder    = OUCH::OUCH50::CancelOrder;
    using OrderRejected  = OUCH::OUCH50::OrderRejected;
    using CancelRejected = OUCH::OUCH50::CancelRejected;

    using Timestamp = OUCH::OUCH50::Timestamp;

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
