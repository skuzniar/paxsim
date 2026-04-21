#ifndef FIX_Modules_FillCorrect_dot_h
#define FIX_Modules_FillCorrect_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/OrderBook.h"
#include "FIX/Context/FillsBook.h"

#include "Common/Params/FillCorrect.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class FillCorrect
{
public:
    template<typename Context>
    explicit FillCorrect(const Config& config, Context& context)
      : m_orderbook(context)
      , m_fillsbook(context)
      , m_params(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate fill corrections if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        correct(next);
    }

private:
    template<typename Next>
    void correct(Next& next)
    {
        using Key = Params::FillCorrect::Key;
        for (const auto& fill : m_fillsbook.fills) {
            if (fill.live()) {
                if (auto itr = m_params.correct.find(Key(fill.quantity(), fill.price())); itr != m_params.correct.end()) {
                    correct(fill, itr->second, next);
                }
            }
        }
    }

    template<typename Next>
    void correct(const Fill& fill, const Config::Table& params, Next& next)
    {
        auto& idx = m_orderbook.orders.get<Context::OrderBook::orderid>();
        auto  itr = idx.find(fill.orderID());
        if (itr == idx.end()) {
            log << level::error << ts << here << ' ' << "Unable to find order for client order ID: " << fill.orderID() << std::endl;
            return;
        }
        return correct(fill, *itr, params, next);
    }

    template<typename Next>
    void correct(const Fill& fill, const Order& order, const Config::Table& params, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        fill.m_quantity = params["NewQuantity"];
        fill.m_price    = params["NewPrice"];
        next.put(m_factory.correct(fill, order, params));
    }

    Context::OrderBook&  m_orderbook;
    Context::FillsBook&  m_fillsbook;
    Params::FillCorrect& m_params;
    Factory              m_factory;
};

} // namespace FIX::Modules
#endif
