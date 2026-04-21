#ifndef FIX_Modules_Factory_FIX42_OrderReject_dot_h
#define FIX_Modules_Factory_FIX42_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Types.h"

#include "Common/Config.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

using Common::Config;

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct OrderReject
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
    explicit OrderReject(Context& context)
      : m_session(context)
    {
    }

    static auto clordID(const NewOrderSingle& msg)
    {
        return Order::ID(msg[tag::ClOrdID]);
    }

    OMessage reject(const NewOrderSingle& msg, const Config::Table& params)
    {
        auto omsg = make(msg_type::ExecutionReport);

        int         code = params["Error"];
        std::string text = params["Message"];

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
        omsg.push_back_string(tag::Price, msg[tag::Price]);
        omsg.push_back_decimal(tag::AvgPx, split(0));
        omsg.push_back_int(tag::OrdRejReason, code);
        omsg.push_back_string(tag::Text, text);

        return omsg.commit();
    }

    static auto clordID(const OrderCancelReplaceRequest& msg)
    {
        return Order::ID(msg[tag::OrigClOrdID]);
    }

    OMessage reject(const OrderCancelReplaceRequest& msg, const Order& order, const Config::Table& params)
    {
        auto omsg = make(msg_type::OrderCancelReject);

        int         code = params["Error"];
        std::string text = params["Message"];

        omsg.push_back_string(tag::OrderID, "NONE");
        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, '8');
        omsg.push_back_char(tag::CxlRejResponseTo, 'G');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, text);

        return omsg.commit();
    }

    static auto clordID(const OrderCancelRequest& msg)
    {
        return Order::ID(msg[tag::OrigClOrdID]);
    }

    OMessage reject(const OrderCancelRequest& msg, const Order& order, const Config::Table& params)
    {
        auto omsg = make(msg_type::OrderCancelReject);

        int         code = params["Error"];
        std::string text = params["Message"];

        omsg.push_back_string(tag::ClOrdID, msg[tag::ClOrdID]);
        omsg.push_back_string(tag::OrigClOrdID, msg[tag::OrigClOrdID]);
        omsg.push_back_char(tag::OrdStatus, std::underlying_type_t<Order::Status>(order.status()));
        omsg.push_back_char(tag::CxlRejResponseTo, 'F');
        omsg.push_back_int(tag::CxlRejReason, code);
        omsg.push_back_string(tag::Text, text);

        return omsg.commit();
    }

private:
    OMessage make(const char* msgtype)
    {
        OMessage omsg(m_buff.data(), m_buff.size(), m_session.beginString());

        omsg.push_back_string(tag::MsgType, msgtype);
        omsg.push_back_string(tag::SenderCompID, m_session.senderCompID());
        omsg.push_back_string(tag::TargetCompID, m_session.targetCompID());
        omsg.push_back_int(tag::MsgSeqNum, m_session.nextOSequence());
        omsg.push_back_timestamp(tag::SendingTime, std::chrono::system_clock::now());

        return omsg;
    }

    Context::Session&      m_session;
    std::array<char, 1024> m_buff;
};

} // namespace FIX::Modules::Factory::FIX42
#endif
