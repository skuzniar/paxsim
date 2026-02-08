#ifndef Factory_OrderFlow_FIX42Inflow_dot_h
#define Factory_OrderFlow_FIX42Inflow_dot_h

#include "Context/Session/Session.h"
#include "Context/OrderBook/OrderBook.h"
#include "Context/ExecutionBook/ExecutionBook.h"

#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelReject.h"

namespace Fix::Factory::OrderFlow {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct FIX42Inflow
{
    using NewOrderSingle            = FIX42::NewOrderSingle;
    using OrderCancelReplaceRequest = FIX42::OrderCancelReplaceRequest;
    using OrderCancelRequest        = FIX42::OrderCancelRequest;
    using ExecutionReport           = FIX42::ExecutionReport;
    using OrderCancelReject         = FIX42::OrderCancelReject;

    using Order = Context::Order;

    template<typename Context>
    explicit FIX42Inflow(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_executionbook(context)
    {
    }

    static Order order(const FIX42::NewOrderSingle& msg)
    {
        return Order(msg.getField(FIX::FIELD::ClOrdID),
                     msg.getField(FIX::FIELD::Symbol),
                     Order::template enum_cast<Order::Side>(msg.getField(FIX::FIELD::Side)),
                     Order::template enum_cast<Order::Type>(msg.getField(FIX::FIELD::OrdType)),
                     std::stoi(msg.getField(FIX::FIELD::OrderQty)),
                     std::stof(msg.getField(FIX::FIELD::Price)));
    }

    FIX42::ExecutionReport reject(const FIX42::NewOrderSingle& msg, int code, std::string& reason)
    {
        auto omsg = make<FIX42::ExecutionReport>();

        omsg.setField(msg.getFieldRef(FIX::FIELD::ClOrdID));
        omsg.setField(msg.getFieldRef(FIX::FIELD::Symbol));
        omsg.setField(msg.getFieldRef(FIX::FIELD::Side));
        omsg.set(FIX::OrderID(Order::anOrderID()));
        omsg.set(FIX::ExecID(Order::anExecutionID()));
        omsg.set(FIX::ExecType(FIX::ExecType_REJECTED));
        omsg.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
        omsg.set(FIX::OrderQty(0));
        omsg.set(FIX::LeavesQty(0));
        omsg.set(FIX::CumQty(0));
        omsg.setField(msg.getFieldRef(FIX::FIELD::Price));
        omsg.set(FIX::AvgPx(0));
        omsg.set(FIX::OrdRejReason(code));
        omsg.set(FIX::Text(reason));
        return omsg;
    }

    FIX42::ExecutionReport accept(const FIX42::NewOrderSingle&, const Order& order)
    {
        auto omsg = make<FIX42::ExecutionReport>();

        omsg.set(FIX::ClOrdID(order.clientOrderID()));
        omsg.set(FIX::Symbol(order.symbol()));
        omsg.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        omsg.set(FIX::OrderID(order.orderID()));
        omsg.set(FIX::ExecID(Order::anExecutionID()));
        omsg.set(FIX::ExecType(FIX::ExecType_NEW));
        omsg.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        omsg.set(FIX::OrderQty(order.quantity()));
        omsg.set(FIX::Price(order.price()));

        set_fills(omsg, order);

        return omsg;
    }

    static Order order(const FIX42::OrderCancelReplaceRequest& msg, const Order& o)
    {
        Order oo(msg.getField(FIX::FIELD::ClOrdID), o.symbol(), o.side(), o.type(), o.quantity(), o.price());
        oo.status(Order::Status::Replaced);
        return oo;
    }

    FIX42::ExecutionReport reject(const FIX42::OrderCancelReplaceRequest& msg, int code, std::string& reason)
    {
        auto omsg = make<FIX42::OrderCancelReject>();

        omsg.set(FIX::OrderID("NONE"));
        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::OrdStatus(FIX::OrdStatus_CANCELED));
        omsg.set(FIX::CxlRejResponseTo('G'));
        omsg.set(FIX::CxlRejReason(code));
        omsg.set(FIX::Text(reason));
        return omsg;
    }

    FIX42::ExecutionReport accept(const FIX42::OrderCancelReplaceRequest& msg, const Order& order)
    {
        auto omsg = make<FIX42::ExecutionReport>();

        omsg.set(FIX::ClOrdID(order.clientOrderID()));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::Symbol(order.symbol()));
        omsg.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        omsg.set(FIX::OrderID(order.orderID()));
        omsg.set(FIX::ExecID(Order::anExecutionID()));
        omsg.set(FIX::ExecType(FIX::ExecType_REPLACED));
        omsg.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        omsg.set(FIX::OrderQty(order.quantity()));
        omsg.set(FIX::Price(order.price()));

        set_fills(omsg, order);

        return omsg;
    }

    FIX42::ExecutionReport reject(const FIX42::OrderCancelRequest& msg, int code, std::string& reason)
    {
        auto omsg = make<FIX42::OrderCancelReject>();

        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
        omsg.set(FIX::CxlRejResponseTo('F'));
        omsg.set(FIX::CxlRejReason(code));
        omsg.set(FIX::Text(reason));
        return omsg;
    }

    FIX42::ExecutionReport accept(const FIX42::OrderCancelRequest& msg, const Order& order)
    {
        auto omsg = make<FIX42::ExecutionReport>();

        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::Symbol(order.symbol()));
        omsg.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        omsg.set(FIX::OrderID(order.orderID()));
        omsg.set(FIX::ExecID(Order::anExecutionID()));
        omsg.set(FIX::ExecType(FIX::ExecType_CANCELED));
        omsg.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        omsg.set(FIX::OrdStatus(FIX::OrdStatus_CANCELED));
        omsg.set(FIX::OrderQty(order.quantity()));
        omsg.set(FIX::Price(order.price()));

        set_fills(omsg, order);

        return omsg;
    }

private:
    template<typename Message>
    Message make()
    {
        Message msg;
        msg.getHeader().set(FIX::SenderCompID(m_session.senderCompID()));
        msg.getHeader().set(FIX::TargetCompID(m_session.targetCompID()));
        msg.getHeader().set(FIX::MsgSeqNum(m_session.nextOSequence()));
        msg.getHeader().set(FIX::SendingTime());
        return std::move(msg);
    }

    template<typename Message>
    void set_fills(Message& message, const Order& order)
    {
        auto [cumqty, avgprx] = m_executionbook.fills(order);

        if (cumqty > 0 && order.status() != Order::Status::Canceled) {
            order.setStatus(cumqty == order.quantity() ? Order::Status::Filled : Order::Status::PartiallyFilled);
        }

        message.set(FIX::OrdStatus(std::underlying_type_t<typename Order::Status>(order.status())));

        message.set(FIX::LeavesQty(order.quantity() - cumqty));
        message.set(FIX::CumQty(cumqty));
        message.set(FIX::AvgPx(avgprx));
    }

    Context::Session&       m_session;
    Context::OrderBook&     m_orderbook;
    Context::ExecutionBook& m_executionbook;
};

} // namespace Fix::Factory::OrderFlow
#endif
