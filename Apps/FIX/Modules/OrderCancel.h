#ifndef FIX_Modules_OrderCancel_dot_h
#define FIX_Modules_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

#include "Common/Config.h"

#include "Params/OrderCancel.h"

namespace FIX::Modules {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderCancel
{
    using Config = Common::Config;

public:
    template<typename Context>
    explicit OrderCancel(Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    template<typename Context>
    OrderCancel(const Config&, Context& context)
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
        for (const auto& order : m_orderbook) {
            for (const auto& [selector, qualifier] : m_params) {
                if (selector.empty() || m_factory.matches(order, selector)) {
                    cancel(order, qualifier, next);
                }
            }
        }
    }

    template<typename Next>
    void cancel(const Order& order, const Config& params, Next& next)
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
