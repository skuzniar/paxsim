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

using namespace OUCH::OUCH50;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderFlow
{
    using PacketHeader    = PacketHeader;
    using SequencedData   = SequencedData;
    using UnsequencedData = UnsequencedData;

    using EnterOrder   = EnterOrder;
    using ReplaceOrder = ReplaceOrder;
    using CancelOrder  = CancelOrder;

    using OrderAccepted  = OrderAccepted;
    using OrderRejected  = OrderRejected;
    using OrderReplaced  = OrderReplaced;
    using OrderCancelled = OrderCancelled;
    using CancelRejected = CancelRejected;

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

    PacketHeader& reject(const EnterOrder& msg, RejectReason reason)
    {
        auto& omsg = *new (m_buff) OrderRejected;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& accept(const EnterOrder& msg, const Order& order)
    {
        auto& omsg = *new (m_buff) OrderAccepted;

        omsg.timestamp                    = Timestamp::now();
        omsg.userRefNum                   = msg.userRefNum;
        omsg.price                        = msg.price;
        omsg.orderReferenceNumber         = 0;
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

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
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

    PacketHeader& reject(const ReplaceOrder& msg, RejectReason reason)
    {
        auto& omsg = *new (m_buff) OrderRejected;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.newUserRefNum;
        omsg.reason     = reason;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& accept(const ReplaceOrder& msg, const Order& order)
    {
        auto& omsg = *new (m_buff) OrderReplaced;

        omsg.timestamp            = Timestamp::now();
        omsg.origUserRefNum       = msg.origUserRefNum;
        omsg.newUserRefNum        = msg.newUserRefNum;
        omsg.price                = order.price();
        omsg.orderReferenceNumber = 0;
        omsg.buySellIndicator     = order.side();
        omsg.orderBook            = order.bookID();
        omsg.quantity             = order.quantity();
        omsg.user                 = msg.user;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    static auto clordID(const CancelOrder& msg)
    {
        return msg.userRefNum;
    }

    PacketHeader& reject(const CancelOrder& msg, RejectReason reason)
    {
        auto& omsg = *new (m_buff) CancelRejected;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& accept(const CancelOrder& msg, const Order& order)
    {
        auto& omsg = *new (m_buff) OrderCancelled;

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::CancelReason::UserRequested;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

private:
    Context::OrderBook& m_orderbook;
    char                m_buff[1024];
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
