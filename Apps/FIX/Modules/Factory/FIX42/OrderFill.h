#ifndef FIX_Modules_Factory_FIX42_OrderFill_dot_h
#define FIX_Modules_Factory_FIX42_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Context/FillsBook.h"

#include "FIX/Types.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class OrderFill
{
public:
    template<typename Context>
    explicit OrderFill(Context& context)
      : m_session(context)
      , m_fillsbook(context)
    {
    }

    OMessage report(const Fill& fill, const Order& o)
    {
        log << level::trace << oflow << here << std::endl;
        auto omsg = make(msg_type::ExecutionReport);

        omsg.push_back_string(tag::ClOrdID, o.clordID());
        omsg.push_back_string(tag::Symbol, o.symbol());
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(o.side()));
        omsg.push_back_string(tag::OrderID, o.orderID());
        omsg.push_back_string(tag::ExecID, fill.executionID());
        omsg.push_back_char(tag::ExecTransType, '0');
        omsg.push_back_int(tag::OrderQty, o.quantity());
        omsg.push_back_decimal(tag::Price, split(o.price()));
        omsg.push_back_int(tag::LastQty, fill.quantity());
        omsg.push_back_decimal(tag::LastPx, split(fill.price()));

        set_fills(omsg, o);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));
        omsg.push_back_char(tag::ExecType, o.status() == Order::Status::Filled ? '2' : '1');

        return omsg.commit();
    }

private:
    OMessage make(const char* msgtype)
    {
        OMessage omsg(m_buff.data(), m_buff.size(), m_session.beginString());

        omsg.push_back_string(tag::MsgType, msgtype);
        omsg.push_back_string(tag::SenderCompID, m_session.senderCompID());
        omsg.push_back_string(tag::TargetCompID, m_session.targetCompID());
        omsg.push_back_int(tag::MsgSeqNum, m_session.oSequenceAdvance());
        omsg.push_back_timestamp(tag::SendingTime, std::chrono::system_clock::now());

        return omsg;
    }

    template<typename Message>
    void set_fills(Message& msg, const Order& o)
    {
        auto [cumqty, avgprx] = m_fillsbook.cumulative(o.orderID());

        if (cumqty > 0 && o.status() != Order::Status::Canceled) {
            o.setStatus(cumqty == o.quantity() ? Order::Status::Filled : Order::Status::PartiallyFilled);
        }

        msg.push_back_int(tag::LeavesQty, o.quantity() - cumqty);
        msg.push_back_int(tag::CumQty, cumqty);
        msg.push_back_decimal(tag::AvgPx, split(avgprx));
    }

    Context::Session&      m_session;
    Context::FillsBook&    m_fillsbook;
    std::array<char, 1024> m_buff;
};

} // namespace FIX::Modules::Factory::FIX42
#endif
