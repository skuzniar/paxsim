#ifndef Factory_ExecutionBook_FIX42Corrections_dot_h
#define Factory_ExecutionBook_FIX42Corrections_dot_h

#include "Context/Session/Session.h"
#include "Context/ExecutionBook/ExecutionBook.h"

#include "quickfix/fix42/ExecutionReport.h"

namespace Fix::Factory::ExecutionBook {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class FIX42Corrections
{
public:
    template<typename Context>
    explicit FIX42Corrections(Context& context)
      : m_session(context)
      , m_executionbook(context)
    {
    }

    using Order     = Context::Order;
    using Execution = Context::Execution;

    FIX42::ExecutionReport report(const Execution& execution, const Order& order)
    {
        auto omsg = make<FIX42::ExecutionReport>();

        omsg.set(FIX::ClOrdID(order.clientOrderID()));
        omsg.set(FIX::Symbol(order.symbol()));
        omsg.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        omsg.set(FIX::OrderID(order.orderID()));
        omsg.set(FIX::ExecID(execution.executionID()));
        omsg.set(FIX::ExecType(FIX::ExecType_NEW));
        omsg.set(FIX::ExecTransType(FIX::ExecTransType_CANCEL));
        omsg.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        omsg.set(FIX::OrderQty(order.quantity()));
        omsg.set(FIX::Price(order.price()));
        omsg.set(FIX::LastShares(execution.quantity()));
        omsg.set(FIX::LastPx(execution.price()));

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
    Context::ExecutionBook& m_executionbook;
};

} // namespace Fix::Modules::ExecutionBook
#endif
