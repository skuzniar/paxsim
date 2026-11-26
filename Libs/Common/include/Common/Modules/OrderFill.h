#ifndef Common_Modules_OrderFill_dot_h
#define Common_Modules_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"
#include "Common/Params/OrderFill.h"

namespace Common::Modules {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order fill handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename OrderBook, typename FillsBook, typename Factory>
class OrderFill
{
public:
    using Config = Common::Config;
    using Params = Common::Params::OrderFill;

    template<typename Context>
    explicit OrderFill(Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    template<typename Context>
    OrderFill(const Config&, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate fills if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << ' ' << here << std::endl;
        fills(next);
    }

private:
    using Order = OrderBook::Order;
    using Fill  = FillsBook::Fill;

    template<typename Next>
    void fills(Next& next)
    {
        log << level::trace << ts << ' ' << here << std::endl;
        for (const auto& order : m_orderbook) {
            for (const auto& [selector, qualifier] : m_params) {
                if (selector.empty() || m_factory.matches(order, selector)) {
                    fills(order, qualifier, next);
                }
            }
        }
    }

    template<typename Next>
    void fills(const Order& order, const Config& fills, Next& next)
    {
        log << level::trace << ts << ' ' << here << std::endl;
        for (const auto& f : fills) {
            fill(order, Config(f), next);
        }
    }

    template<typename Next>
    void fill(const Order& order, const Config& fill, Next& next)
    {
        log << level::trace << ts << ' ' << here << ' ' << fill << std::endl;
        Fill f = Fill(order, static_cast<int>(fill["Quantity"]), static_cast<double>(fill["Price"]));

        auto [cumqty, avgprx] = m_fillsbook.cumulative(order.orderID());
        if (order.quantity() - cumqty >= f.quantity()) {
            if (!m_fillsbook.fills.insert(f).second) {
                log << level::error << ts << here << ' ' << "Unable to record the execution for order: " << order.clordID() << std::endl;
                return;
            }
            next.put(m_factory.report(f, order));
        }
    }

    OrderBook& m_orderbook;
    FillsBook& m_fillsbook;
    Params&    m_params;
    Factory    m_factory;
};

} // namespace Common::Modules
#endif
