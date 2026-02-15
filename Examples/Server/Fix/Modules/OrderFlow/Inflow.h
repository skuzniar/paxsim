#ifndef Modules_OrderFlow_Inflow_dot_h
#define Modules_OrderFlow_Inflow_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context/Session/Session.h"
#include "Context/OrderBook/OrderBook.h"
#include "Context/ExecutionBook/ExecutionBook.h"
#include "Utils.h"

#include <quickfix/Message.h>

namespace Fix::Modules::OrderFlow {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Inflow
{
public:
    template<typename Context>
    explicit Inflow(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_executionbook(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    bool put(const FIX::Message& msg, Next& next)
    {
        log << level::debug << vmark << '[' << fixdump(msg.toString()) << ']' << std::endl;

        const auto& msgtype = msg.getHeader().getField(FIX::FIELD::MsgType);
        if (msgtype == FIX::MsgType_NewOrderSingle) {
            return process(typename Factory::NewOrderSingle(msg), next);
        }
        if (msgtype == FIX::MsgType_OrderCancelReplaceRequest) {
            return process(typename Factory::OrderCancelReplaceRequest(msg), next);
        }
        if (msgtype == FIX::MsgType_OrderCancelRequest) {
            return process(typename Factory::OrderCancelRequest(msg), next);
        }
        return true;
    }

private:
    using Order   = Context::Order;
    using clordid = Context::OrderBook::clordid;

    template<typename Next>
    bool process(const Factory::NewOrderSingle& msg, Next& next)
    {
        auto o = m_factory.order(msg);
        if (!m_orderbook.orders.insert(o).second) {
            std::string error = "Duplicate client order ID: " + o.clientOrderID();
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, FIX::OrdRejReason_DUPLICATE_ORDER, error));
        }
        return next.put(m_factory.accept(msg, o));
    }

    template<typename Next>
    bool process(const Factory::OrderCancelReplaceRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();

        auto itr = idx.find(msg.getField(FIX::FIELD::OrigClOrdID));
        if (itr == idx.end()) {
            std::string error = "Unknown client order ID: " + msg.getField(FIX::FIELD::OrigClOrdID);
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, FIX::OrdRejReason_UNKNOWN_ORDER, error));
        }

        auto o = m_factory.order(msg, *itr);
        idx.erase(itr);
        if (!m_orderbook.orders.insert(o).second) {
            std::string error = "Duplicate client order ID: " + o.clientOrderID();
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, FIX::OrdRejReason_DUPLICATE_ORDER, error));
        }
        return next.put(m_factory.accept(msg, o));
    }

    template<typename Next>
    bool process(const Factory::OrderCancelRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        auto  itr = idx.find(msg.getField(FIX::FIELD::OrigClOrdID));
        if (itr == idx.end()) {
            std::string error = "Unknown client order ID: " + msg.getField(FIX::FIELD::OrigClOrdID);
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, FIX::OrdRejReason_UNKNOWN_ORDER, error));
        }

        itr->status(Order::Status::Canceled);
        auto ret = next.put(m_factory.accept(msg, *itr));
        idx.erase(itr);

        return ret;
    }

    Context::Session&       m_session;
    Context::OrderBook&     m_orderbook;
    Context::ExecutionBook& m_executionbook;
    Factory                 m_factory;
};

} // namespace Fix::Modules::OrderFlow
#endif
