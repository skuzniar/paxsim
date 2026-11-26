#ifndef Common_Modules_FillCancel_dot_h
#define Common_Modules_FillCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"
#include "Common/Params/FillCancel.h"

namespace Common::Modules {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename OrderBook, typename FillsBook, typename Factory>
class FillCancel
{
public:
    using Config = Common::Config;
    using Params = Common::Params::FillCancel;

    template<typename Context>
    explicit FillCancel(Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    template<typename Context>
    FillCancel(const Config&, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine fills book and generate fill cancellations if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << ' ' << here << std::endl;
        cancel(next);
    }

private:
    using Order = OrderBook::Order;
    using Fill  = FillsBook::Fill;

    template<typename Next>
    void cancel(Next& next)
    {
        for (const auto& fill : m_fillsbook) {
            if (fill.live()) {
                for (const auto& [selector, qualifier] : m_params) {
                    if (selector.empty() || m_factory.matches(fill, selector)) {
                        cancel(fill, qualifier, next);
                    }
                }
            }
        }
    }

    template<typename Next>
    void cancel(const Fill& fill, const Config& params, Next& next)
    {
        auto itr = m_orderbook.ByOrderID.find(fill.orderID());
        if (itr == m_orderbook.ByOrderID.end()) {
            log << level::error << ts << here << ' ' << "Unable to find order for client order ID: " << fill.orderID() << std::endl;
            return;
        }
        return cancel(fill, *itr, params, next);
    }

    template<typename Next>
    void cancel(const Fill& fill, const Order& order, const Config& params, Next& next)
    {
        fill.cancel();
        next.put(m_factory.cancel(fill, order, params));
    }

    OrderBook& m_orderbook;
    FillsBook& m_fillsbook;
    Params&    m_params;
    Factory    m_factory;
};

} // namespace Common::Modules
#endif
