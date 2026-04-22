#ifndef FIX_Modules_Factory_FIX42_OrderFlow_dot_h
#define FIX_Modules_Factory_FIX42_OrderFlow_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"
#include "FIX/Types.h"
#include "FIX/Utils.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderFlow
{
    struct NewOrderSingle : public IMessage
    {
        static auto type()
        {
            return msg_type::NewOrderSingle;
        }
    };

    struct OrderCancelReplaceRequest : public IMessage
    {
        static auto type()
        {
            return msg_type::OrderCancelReplaceRequest;
        }
    };

    struct OrderCancelRequest : public IMessage
    {
        static auto type()
        {
            return msg_type::OrderCancelRequest;
        }
    };

    template<typename Context>
    OrderFlow(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_fillsbook(context)
    {
    }

    static auto clordID(const NewOrderSingle& msg)
    {
        return Order::ID(msg[tag::ClOrdID]);
    }

    static Order order(const NewOrderSingle& msg)
    {
        return Order(std::string(msg[tag::ClOrdID]),
                     std::string(msg[tag::Symbol]),
                     Order::template enum_cast<Order::Side>(msg[tag::Side]),
                     Order::template enum_cast<Order::Type>(msg[tag::OrdType]),
                     to_int<int>(msg[tag::OrderQty]),
                     to_float<double>(msg[tag::Price]));
    }

    OMessage reject(const NewOrderSingle& msg, int code, std::string& reason)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::ExecutionReport);

        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::Symbol, msg[tag::Symbol]);
        omsg.push_back_string(tag::Side, msg[tag::Side]);
        omsg.push_back_string(tag::OrderID, Order::anOrderID());
        omsg.push_back_string(tag::ExecID, Order::anExecutionID());
        omsg.push_back_char(tag::ExecType, '8');
        omsg.push_back_char(tag::ExecTransType, '0');
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(Order::Status::Rejected));
        omsg.push_back_int(tag::OrderQty, 0);
        omsg.push_back_int(tag::LeavesQty, 0);
        omsg.push_back_int(tag::CumQty, 0);
        omsg.push_back_decimal(tag::Price, split(to_float(msg[tag::Price])));
        omsg.push_back_decimal(tag::AvgPx, split(0));
        omsg.push_back_int(tag::OrdRejReason, code);
        omsg.push_back_string(tag::Text, reason);

        return omsg.commit();
    }

    OMessage accept(const NewOrderSingle&, const Order& o)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::ExecutionReport);

        omsg.push_back_string(tag::ClOrdID, o.clordID());
        omsg.push_back_string(tag::Symbol, o.symbol());
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(o.side()));
        omsg.push_back_string(tag::OrderID, o.orderID());
        omsg.push_back_string(tag::ExecID, Order::anExecutionID());
        omsg.push_back_char(tag::ExecType, '0');
        omsg.push_back_char(tag::ExecTransType, '0');
        omsg.push_back_int(tag::OrderQty, o.quantity());
        omsg.push_back_decimal(tag::Price, split(o.price()));

        set_fills(omsg, o);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));

        return omsg.commit();
    }

    static auto clordID(const OrderCancelReplaceRequest& msg)
    {
        return Order::ID(msg[tag::OrigClOrdID]);
    }

    static Order order(const OrderCancelReplaceRequest& msg, const Order& o)
    {
        Order order(
            std::string(msg[tag::ClOrdID]), o.symbol(), o.side(), o.type(), to_int(msg[tag::OrderQty], o.quantity()), to_float(msg[tag::Price], o.price()));
        order.status(Order::Status::Replaced);
        return order;
    }

    OMessage reject(const OrderCancelReplaceRequest& msg, int code, std::string& reason)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::OrderCancelReject);

        omsg.push_back_string(tag::OrderID, "NONE");
        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(Order::Status::Rejected));
        omsg.push_back_char(tag::CxlRejResponseTo, 'G');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, reason);

        return omsg.commit();
    }

    OMessage reject(const OrderCancelReplaceRequest& msg, const Order& o, int code, std::string& reason)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::OrderCancelReject);

        omsg.push_back_string(tag::OrderID, "NONE");
        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));
        omsg.push_back_char(tag::CxlRejResponseTo, 'G');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, reason);

        return omsg.commit();
    }

    OMessage accept(const OrderCancelReplaceRequest& msg, const Order& o)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::ExecutionReport);

        omsg.push_back_string(tag::ClOrdID, o.clordID());
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_string(tag::Symbol, o.symbol());
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(o.side()));
        omsg.push_back_string(tag::OrderID, o.orderID());
        omsg.push_back_string(tag::ExecID, Order::anExecutionID());
        omsg.push_back_char(tag::ExecType, '5');
        omsg.push_back_char(tag::ExecTransType, '0');
        omsg.push_back_int(tag::OrderQty, o.quantity());
        omsg.push_back_decimal(tag::Price, split(o.price()));

        set_fills(omsg, o);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));

        return omsg.commit();
    }

    static auto clordID(const OrderCancelRequest& msg)
    {
        return Order::ID(msg[tag::OrigClOrdID]);
    }

    OMessage reject(const OrderCancelRequest& msg, int code, std::string& reason)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::OrderCancelReject);

        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(Order::Status::Rejected));
        omsg.push_back_char(tag::CxlRejResponseTo, 'F');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, reason);

        return omsg.commit();
    }

    OMessage reject(const OrderCancelRequest& msg, const Order& o, int code, std::string& reason)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::OrderCancelReject);

        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));
        omsg.push_back_char(tag::CxlRejResponseTo, 'F');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, reason);

        return omsg.commit();
    }

    OMessage accept(const OrderCancelRequest& msg, const Order& o)
    {
        log << level::trace << vmark << here << std::endl;
        auto omsg = make(msg_type::ExecutionReport);

        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_string(tag::Symbol, o.symbol());
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(o.side()));
        omsg.push_back_string(tag::OrderID, o.orderID());
        omsg.push_back_string(tag::ExecID, Order::anExecutionID());
        omsg.push_back_char(tag::ExecType, '4');
        omsg.push_back_char(tag::ExecTransType, '0');
        omsg.push_back_int(tag::OrderQty, o.quantity());
        omsg.push_back_decimal(tag::Price, split(o.price()));

        set_fills(omsg, o);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(o.status()));

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
    Context::OrderBook&    m_orderbook;
    Context::FillsBook&    m_fillsbook;
    std::array<char, 1024> m_buff;
};

} // namespace FIX::Modules::Factory::FIX42
#endif
