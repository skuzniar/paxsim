#ifndef OUCH_Modules_OrderCancelOnDisconnect_dot_h
#define OUCH_Modules_OrderCancelOnDisconnect_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/Session.h"
#include "OUCH/Context/OrderBook.h"
#include "OUCH/Context/FillsBook.h"

#include "Common/Config.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order cancel handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderCancelOnDisconnect
{
public:
    using State = Context::Session::State;

    template<typename Context>
    OrderCancelOnDisconnect(const Config& config, Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_fillsbook(context)
      , m_factory(context)
    {
    }

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
        log << level::trace << ts << here << std::endl;
        for (const auto& order : m_orderbook.orders) {
            if (order.status() != Order::Status::Canceled && order.status() != Order::Status::Rejected) {
                cancel(order, next);
            }
        }
    }

    template<typename Next>
    void cancel(const Order& order, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        order.status(Order::Status::Canceled);
        next.put(m_factory.cancel(order));
    }

    Context::Session&   m_session;
    Context::OrderBook& m_orderbook;
    Context::FillsBook& m_fillsbook;
    Factory             m_factory;

    // Run cancelation once per session
    bool m_hot = true;
};

} // namespace OUCH::Modules
#endif
