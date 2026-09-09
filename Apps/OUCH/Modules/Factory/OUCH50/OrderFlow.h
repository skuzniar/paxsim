#ifndef OUCH_Modules_Factory_OUCH50_OrderFlow_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderFlow_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/OrderBook.h"

#include "OUCH/OUCH50/EnterOrder.h"
#include "OUCH/OUCH50/ReplaceOrder.h"
#include "OUCH/OUCH50/CancelOrder.h"

#include "OUCH/OUCH50/OrderAccepted.h"
#include "OUCH/OUCH50/OrderRejected.h"
#include "OUCH/OUCH50/OrderReplaced.h"
#include "OUCH/OUCH50/OrderCancelled.h"
#include "OUCH/OUCH50/CancelRejected.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderFlow
{
    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;

    using EnterOrder   = OUCH::OUCH50::EnterOrder;
    using ReplaceOrder = OUCH::OUCH50::ReplaceOrder;
    using CancelOrder  = OUCH::OUCH50::CancelOrder;

    using OrderAccepted  = OUCH::OUCH50::OrderAccepted;
    using OrderRejected  = OUCH::OUCH50::OrderRejected;
    using OrderReplaced  = OUCH::OUCH50::OrderReplaced;
    using OrderCancelled = OUCH::OUCH50::OrderCancelled;
    using CancelRejected = OUCH::OUCH50::CancelRejected;

    using RejectReason = OUCH::OUCH50::RejectReason;
    using Timestamp    = OUCH::OUCH50::Timestamp;

    template<typename Context>
    explicit OrderFlow(Context& context)
      : m_orderbook(context)
    {
    }

    static auto clordID(const EnterOrder& msg)
    {
        return msg.userRefNum;
    }

    static Order order(const EnterOrder& msg)
    {
        return Order(msg.userRefNum, msg.buySellIndicator, msg.orderBook, msg.quantity, msg.price);
    }

    auto reject(const EnterOrder& msg, RejectReason reason)
    {
        OrderRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto accept(const EnterOrder& msg, const Order& order)
    {
        OrderAccepted omsg;

        omsg.timestamp                    = Timestamp::now();
        omsg.userRefNum                   = msg.userRefNum;
        omsg.price                        = msg.price;
        omsg.orderReferenceNumber         = order.orderID();
        omsg.buySellIndicator             = msg.buySellIndicator;
        omsg.orderBook                    = msg.orderBook;
        omsg.quantity                     = msg.quantity;
        omsg.user                         = msg.user;
        omsg.executionWithinFirm          = msg.executionWithinFirm;
        omsg.investmentDecisionWithinFirm = msg.investmentDecisionWithinFirm;
        omsg.clientIdentifier             = msg.clientIdentifier;
        omsg.partyRoleQualifier           = msg.partyRoleQualifier;
        omsg.capacity                     = msg.capacity;
        omsg.algoIndicator                = msg.algoIndicator;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    static auto clordID(const ReplaceOrder& msg)
    {
        return msg.origUserRefNum;
    }

    static Order order(const ReplaceOrder& msg, const Order& o)
    {
        Order oo(msg.newUserRefNum, o.side(), o.bookID(), msg.quantity, msg.price);
        return oo;
    }

    auto reject(const ReplaceOrder& msg, RejectReason reason)
    {
        OrderRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.newUserRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto accept(const ReplaceOrder& msg, const Order& order)
    {
        OrderReplaced omsg;

        omsg.timestamp            = Timestamp::now();
        omsg.origUserRefNum       = msg.origUserRefNum;
        omsg.newUserRefNum        = msg.newUserRefNum;
        omsg.price                = order.price();
        omsg.orderReferenceNumber = order.orderID();
        omsg.buySellIndicator     = order.side();
        omsg.orderBook            = order.bookID();
        omsg.quantity             = order.quantity();
        omsg.user                 = msg.user;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    static auto clordID(const CancelOrder& msg)
    {
        return msg.userRefNum;
    }

    auto reject(const CancelOrder& msg, RejectReason reason)
    {
        CancelRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto accept(const CancelOrder& msg, const Order& order)
    {
        OrderCancelled omsg;

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::CancelReason::UserRequested;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

private:
    Context::OrderBook& m_orderbook;
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
