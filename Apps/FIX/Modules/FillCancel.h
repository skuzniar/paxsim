#ifndef FIX_Modules_FillCancel_dot_h
#define FIX_Modules_FillCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

#include "Common/Params/FillCancel.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class FillCancel
{
public:
    template<typename Context>
    FillCancel(const Config& config, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate fill cancellations if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        cancel(next);
    }

private:
    template<typename Next>
    void cancel(Next& next)
    {
        using Key = Params::FillCancel::Key;
        for (const auto& fill : m_fillsbook.fills) {
            if (fill.live()) {
                if (auto itr = m_params.cancel.find(Key(fill.quantity(), fill.price())); itr != m_params.cancel.end()) {
                    cancel(fill, itr->second, next);
                }
            }
        }
    }

    template<typename Next>
    void cancel(const Fill& fill, const Config::Table& params, Next& next)
    {
        auto& idx = m_orderbook.orders.get<Context::OrderBook::orderid>();
        auto  itr = idx.find(fill.orderID());
        if (itr == idx.end()) {
            log << level::error << ts << here << ' ' << "Unable to find order for client order ID: " << fill.orderID() << std::endl;
            return;
        }
        return cancel(fill, *itr, params, next);
    }

    template<typename Next>
    void cancel(const Fill& fill, const Order& order, const Config::Table& params, Next& next)
    {
        fill.cancel();
        next.put(m_factory.cancel(fill, order, params));
    }

    Context::OrderBook& m_orderbook;
    Context::FillsBook& m_fillsbook;
    Params::FillCancel& m_params;
    Factory             m_factory;
};

} // namespace FIX::Modules
#endif
