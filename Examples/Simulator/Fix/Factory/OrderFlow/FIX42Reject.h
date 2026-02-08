#ifndef Factory_OrderFlow_FIX42Reject_dot_h
#define Factory_OrderFlow_FIX42Reject_dot_h

#include "Context/Session/Session.h"
#include "Context/OrderBook/Order.h"

#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelReject.h"

namespace Fix::Factory::OrderFlow {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct FIX42Reject
{
    using NewOrderSingle            = FIX42::NewOrderSingle;
    using OrderCancelReplaceRequest = FIX42::OrderCancelReplaceRequest;
    using OrderCancelRequest        = FIX42::OrderCancelRequest;
    using ExecutionReport           = FIX42::ExecutionReport;
    using OrderCancelReject         = FIX42::OrderCancelReject;

    using Order = Context::Order;

    template<typename Context>
    explicit FIX42Reject(Context& context)
      : m_session(context)
    {
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

    FIX42::ExecutionReport reject(const FIX42::OrderCancelReplaceRequest& msg, int code, std::string& reason)
    {
        auto omsg = make<FIX42::OrderCancelReject>();

        omsg.set(FIX::OrderID("NONE"));
        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::OrdStatus(FIX::OrdStatus_CANCELED));
        omsg.set(FIX::CxlRejResponseTo('A'));
        omsg.set(FIX::CxlRejReason(code));
        omsg.set(FIX::Text(reason));
        return omsg;
    }

    FIX42::ExecutionReport reject(const FIX42::OrderCancelReplaceRequest& msg, const Order& order, int code, std::string& reason)
    {
        auto omsg = make<FIX42::OrderCancelReject>();

        omsg.set(FIX::OrderID(order.orderID()));
        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        omsg.setField(FIX::CxlRejResponseTo('G'));
        omsg.set(FIX::CxlRejReason(code));
        omsg.set(FIX::Text(reason));
        return omsg;
    }

    FIX42::ExecutionReport reject(const FIX42::OrderCancelRequest& msg, const Order& order, int code, std::string& reason)
    {
        auto omsg = make<FIX42::OrderCancelReject>();

        omsg.set(FIX::ClOrdID(msg.getField(FIX::FIELD::ClOrdID)));
        omsg.set(FIX::OrigClOrdID(msg.getField(FIX::FIELD::OrigClOrdID)));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        omsg.set(FIX::CxlRejResponseTo('F'));
        omsg.set(FIX::CxlRejReason(code));
        omsg.set(FIX::Text(reason));
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

    Context::Session& m_session;
};

} // namespace Fix::Factory::OrderFlow
#endif
