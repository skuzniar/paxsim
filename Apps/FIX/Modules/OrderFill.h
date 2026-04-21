#ifndef FIX_Modules_OrderFill_dot_h
#define FIX_Modules_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

#include "Common/Params/OrderFill.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderFill
{
public:
    template<typename Context>
    explicit OrderFill(const Config& config, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate fills cancells and corrections if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        fills(next);
    }

private:
    template<typename Next>
    void fills(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        for (const auto& order : m_orderbook.orders) {
            if (auto itr = m_params.fills.find(order.quantity()); itr != m_params.fills.end()) {
                if (order.status() != Order::Status::Filled && order.status() != Order::Status::PartiallyFilled) {
                    fill(order, itr->second, next);
                }
            }
        }
    }

    template<typename Next>
    void fill(const Order& order, const Params::OrderFill::Fills& fills, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        for (const auto& f : fills) {
            fill(order, f, next);
        }
    }

    template<typename Next>
    void fill(const Order& order, const Config::Table& fill, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        Fill f = Fill(order, fill["Quantity"], fill["Price"]);

        auto [cumqty, avgprx] = m_fillsbook.cumulative(order.orderID());
        if (order.quantity() - cumqty > f.quantity()) {
            if (!m_fillsbook.fills.insert(f).second) {
                log << level::error << ts << here << ' ' << "Unable to record the execution for order: " << order.clordID() << std::endl;
                return;
            }
            log << level::debug << ts << here << ' ' << f << std::endl;
            next.put(m_factory.report(f, order));
        }
    }

    Context::OrderBook& m_orderbook;
    Context::FillsBook& m_fillsbook;
    Params::OrderFill&  m_params;
    Factory             m_factory;
};

} // namespace FIX::Modules
#endif
