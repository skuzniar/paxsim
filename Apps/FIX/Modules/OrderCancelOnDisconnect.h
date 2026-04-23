#ifndef FIX_Modules_OrderCancelOnDisconnect_dot_h
#define FIX_Modules_OrderCancelOnDisconnect_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include "FIX/Context/Session.h"
#include "FIX/Context/OrderBook.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order Book cancel on disconnect module
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderCancelOnDisconnect
{
public:
    using State = Context::Session::State;

    template<typename Context>
    explicit OrderCancelOnDisconnect(const Config& config, Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_factory(context)
    {
    }

    // Examine order book and generate order cancells if necessary
    template<typename Next>
    void eval(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        if (m_session.state() == State::Normal && m_hot) {
            cancell(next);
            m_hot = false;
        }
    }

private:
    template<typename Next>
    void cancell(Next& next)
    {
        for (const auto& order : m_orderbook.orders) {
            if (order.status() != Order::Status::Canceled && order.status() != Order::Status::Rejected) {
                cancel(order, next);
            }
        }
    }

    template<typename Next>
    void cancel(const Order& order, Next& next)
    {
        order.status(Order::Status::Canceled);
        next.put(m_factory.cancel(order));
    }

    Context::Session&   m_session;
    Context::OrderBook& m_orderbook;
    Factory             m_factory;

    // Run cancelation once per session
    bool m_hot = true;
};

} // namespace FIX::Modules
#endif
