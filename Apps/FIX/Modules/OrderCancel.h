#ifndef FIX_Modules_OrderCancel_dot_h
#define FIX_Modules_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

#include "Common/Params/OrderCancel.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order Book cancellation module
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderCancel
{
public:
    template<typename Context>
    explicit OrderCancel(const Config& config, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate fill cancells if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        cancell(next);
    }

private:
    template<typename Next>
    void cancell(Next& next)
    {
        for (const auto& order : m_orderbook.orders) {
            if (auto itr = m_params.quantity.find(order.quantity()); itr != m_params.quantity.end()) {
                if (order.status() != Order::Status::Canceled) {
                    cancel(order, itr->second, next);
                }
            }
        }
        for (const auto& order : m_orderbook.orders) {
            auto [cumqty, avgprx] = m_fillsbook.cumulative(order.orderID());
            auto leaves           = order.quantity() - cumqty;
            if (auto itr = m_params.leaves.find(order.quantity()); itr != m_params.leaves.end()) {
                if (order.status() != Order::Status::Canceled) {
                    cancel(order, itr->second, next);
                }
            }
            // Scan below quantity parameters in ascending order to see if we get hits
            for (const auto& [below, params] : m_params.below) {
                if (leaves < below) {
                    if (order.status() != Order::Status::Canceled) {
                        cancel(order, params, next);
                    }
                }
            }
        }
    }

    template<typename Next>
    void cancel(const Order& order, const Config::Table& params, Next& next)
    {
        order.status(Order::Status::Canceled);
        next.put(m_factory.cancel(order, params));
    }

    Context::OrderBook&  m_orderbook;
    Context::FillsBook&  m_fillsbook;
    Params::OrderCancel& m_params;
    Factory              m_factory;
};

} // namespace FIX::Modules
#endif
