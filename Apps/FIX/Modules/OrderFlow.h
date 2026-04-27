#ifndef FIX_Modules_OrderFlow_dot_h
#define FIX_Modules_OrderFlow_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"
#include "FIX/Types.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderFlow
{
public:
    using NewOrderSingle            = Factory::NewOrderSingle;
    using OrderCancelReplaceRequest = Factory::OrderCancelReplaceRequest;
    using OrderCancelRequest        = Factory::OrderCancelRequest;

    template<typename Context>
    OrderFlow(const Config& config, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_factory(context)
    {
    }

    template<typename Message, typename Next>
    bool put(const Message& msg, Next& next)
    {
        log << level::debug << oflow << '[' << msg << ']' << std::endl;

        auto msgtype = msg.msgtype();

        if (msgtype == NewOrderSingle::type()) {
            return process(NewOrderSingle(msg), next);
        }
        if (msgtype == OrderCancelReplaceRequest::type()) {
            return process(OrderCancelReplaceRequest(msg), next);
        }
        if (msgtype == OrderCancelRequest::type()) {
            return process(OrderCancelRequest(msg), next);
        }
        return true;
    }

private:
    using clordid = Context::OrderBook::clordid;

    template<typename Next>
    bool process(const NewOrderSingle& msg, Next& next)
    {
        log << level::trace << oflow << here << std::endl;
        auto o = m_factory.order(msg);
        if (!m_orderbook.orders.insert(o).second) {
            std::string error = "Duplicate client order ID: " + o.clordID();
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, 6, error));
        }
        log << level::debug << ts << here << ' ' << o << std::endl;
        return next.put(m_factory.accept(msg, o));
    }

    template<typename Next>
    bool process(const OrderCancelReplaceRequest& msg, Next& next)
    {
        log << level::trace << oflow << here << std::endl;
        auto& idx = m_orderbook.orders.get<clordid>();

        auto itr = idx.find(Factory::clordID(msg));
        if (itr == idx.end()) {
            std::string error = "Unknown client order ID: " + Factory::clordID(msg);
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, 5, error));
        }

        auto o = m_factory.order(msg, *itr);
        idx.erase(itr);
        if (!m_orderbook.orders.insert(o).second) {
            std::string error = "Duplicate client order ID: " + o.clordID();
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, o, 6, error));
        }
        return next.put(m_factory.accept(msg, o));
    }

    template<typename Next>
    bool process(const OrderCancelRequest& msg, Next& next)
    {
        log << level::trace << oflow << here << std::endl;
        auto& idx = m_orderbook.orders.get<clordid>();
        auto  itr = idx.find(Factory::clordID(msg));
        if (itr == idx.end()) {
            std::string error = "Unknown client order ID: " + std::string(Factory::clordID(msg));
            log << level::error << ts << here << ' ' << error << std::endl;
            return next.put(m_factory.reject(msg, 5, error));
        }

        itr->status(Order::Status::Canceled);
        auto ret = next.put(m_factory.accept(msg, *itr));
        idx.erase(itr);

        return ret;
    }

    Context::OrderBook& m_orderbook;
    Context::FillsBook& m_fillsbook;
    Factory             m_factory;
};

} // namespace FIX::Modules
#endif
