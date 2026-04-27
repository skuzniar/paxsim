#ifndef FIX_Modules_Factory_FIX42_Feeder_dot_h
#define FIX_Modules_Factory_FIX42_Feeder_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Types.h"
#include "FIX/Utils.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;
//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class Feeder
{
public:
    template<typename Context>
    explicit Feeder(Context& context)
      : m_session(context)
    {
    }

    OMessage enterOrder()
    {
        log << level::trace << oflow << here << std::endl;
        auto omsg = make(msg_type::NewOrderSingle);

        m_clordid = Order::anOrderID();

        omsg.push_back_string(tag::ClOrdID, m_clordid = Order::anOrderID());
        omsg.push_back_string(tag::Symbol, "AAPL");
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(Order::Side::Buy));
        omsg.push_back_char(tag::OrdType, std::underlying_type_t<Order::Type>(Order::Type::Limit));
        omsg.push_back_int(tag::OrderQty, 1000);
        omsg.push_back_decimal(tag::Price, split(4.25));

        return omsg.commit();
    }

    OMessage replaceOrder()
    {
        log << level::trace << oflow << here << std::endl;
        auto omsg = make(msg_type::OrderCancelReplaceRequest);

        omsg.push_back_string(tag::OrigClOrdID, m_clordid);
        omsg.push_back_string(tag::ClOrdID, m_clordid = Order::anOrderID());
        omsg.push_back_string(tag::Symbol, "AAPL");
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(Order::Side::Buy));
        omsg.push_back_char(tag::OrdType, std::underlying_type_t<Order::Type>(Order::Type::Limit));
        omsg.push_back_int(tag::OrderQty, 2000);
        omsg.push_back_decimal(tag::Price, split(5.25));

        return omsg.commit();
    }

    OMessage cancelOrder()
    {
        log << level::trace << oflow << here << std::endl;
        auto omsg = make(msg_type::OrderCancelRequest);

        omsg.push_back_string(tag::OrigClOrdID, m_clordid);
        omsg.push_back_string(tag::ClOrdID, m_clordid = Order::anOrderID());
        omsg.push_back_string(tag::Symbol, "AAPL");
        omsg.push_back_char(tag::Side, std::underlying_type_t<Order::Side>(Order::Side::Buy));
        omsg.push_back_int(tag::OrderQty, 2000);

        return omsg.commit();
        return omsg;
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

    Context::Session&      m_session;
    std::array<char, 1024> m_buff;

    // Keep some details across calls
    std::string m_clordid;
};

} // namespace FIX::Modules::Factory::FIX42
#endif
