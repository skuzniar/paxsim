#ifndef FIX_Modules_OrderReject_dot_h
#define FIX_Modules_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"
#include "Common/Params/OrderReject.h"

#include "FIX/Context/OrderBook.h"

namespace FIX::Modules {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order reject handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderReject
{
    using Config = Common::Config;
    using Params = Common::Params::OrderReject;

public:
    using NewOrderSingle            = Factory::NewOrderSingle;
    using OrderCancelReplaceRequest = Factory::OrderCancelReplaceRequest;
    using OrderCancelRequest        = Factory::OrderCancelRequest;

    template<typename Context>
    explicit OrderReject(Context& context)
      : m_orderbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    template<typename Context>
    OrderReject(const Config&, Context& context)
      : m_orderbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    template<typename Message, typename Next>
    bool put(const Message& msg, Next& next)
    {
        log << level::debug << oflow << '[' << msg << ']' << std::endl;

        auto msgtype = msg.msgtype();

        if (msgtype == NewOrderSingle::type()) {
            return reject(NewOrderSingle(msg), next);
        }
        if (msgtype == OrderCancelReplaceRequest::type()) {
            return reject(OrderCancelReplaceRequest(msg), next);
        }
        if (msgtype == OrderCancelRequest::type()) {
            return reject(OrderCancelRequest(msg), next);
        }
        return true;
    }

private:
    using clordid = Context::OrderBook::clordid;

    template<typename Next>
    bool reject(const NewOrderSingle& msg, Next& next)
    {
        for (const auto& [selector, qualifier] : m_params.enter) {
            if (selector.empty() || m_factory.matches(msg, selector)) {
                next.put(m_factory.reject(msg, qualifier));
                return false;
            }
        }
        return true;
    }

    template<typename Next>
    bool reject(const OrderCancelReplaceRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            for (const auto& [selector, qualifier] : m_params.replace) {
                if (selector.empty() || m_factory.matches(*itr, selector)) {
                    next.put(m_factory.reject(msg, *itr, qualifier));
                    return false;
                }
            }
        }
        return true;
    }

    template<typename Next>
    bool reject(const OrderCancelRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            for (const auto& [selector, qualifier] : m_params.cancel) {
                if (selector.empty() || m_factory.matches(*itr, selector)) {
                    next.put(m_factory.reject(msg, *itr, qualifier));
                    return false;
                }
            }
        }
        return true;
    }

    Context::OrderBook& m_orderbook;
    Params&             m_params;
    Factory             m_factory;
};

} // namespace FIX::Modules
#endif
