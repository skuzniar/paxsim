#ifndef FIX_Modules_OrderReject_dot_h
#define FIX_Modules_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
// #include "FIX/Utils.h"

#include "Common/Params/OrderReject.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order reject handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderReject
{
public:
    using NewOrderSingle            = Factory::NewOrderSingle;
    using OrderCancelReplaceRequest = Factory::OrderCancelReplaceRequest;
    using OrderCancelRequest        = Factory::OrderCancelRequest;

    template<typename Context>
    OrderReject(const Config& config, Context& context)
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
        auto quantity = to_int<int>(msg[tag::OrderQty]);
        if (auto pit = m_params.enter.find(quantity); pit != m_params.enter.end()) {
            next.put(m_factory.reject(msg, pit->second));
            return false;
        }
        return true;
    }

    template<typename Next>
    bool reject(const OrderCancelReplaceRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            int quantity = itr->quantity();
            if (auto pit = m_params.replace.find(quantity); pit != m_params.replace.end()) {
                next.put(m_factory.reject(msg, *itr, pit->second));
                return false;
            }
        }
        return true;
    }

    template<typename Next>
    bool reject(const OrderCancelRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            int quantity = itr->quantity();
            if (auto pit = m_params.cancel.find(quantity); pit != m_params.cancel.end()) {
                next.put(m_factory.reject(msg, *itr, pit->second));
                return false;
            }
        }
        return true;
    }

    Context::OrderBook&  m_orderbook;
    Params::OrderReject& m_params;
    Factory              m_factory;
};

} // namespace FIX::Modules
#endif
