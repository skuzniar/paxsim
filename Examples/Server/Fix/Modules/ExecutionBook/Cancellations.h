#ifndef Modules_ExecutionBook_Cancellations_dot_h
#define Modules_ExecutionBook_Cancellations_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context/Session/Session.h"
#include "Context/OrderBook/OrderBook.h"
#include "Context/ExecutionBook/ExecutionBook.h"

#include <quickfix/Message.h>

namespace Fix::Modules::ExecutionBook {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Cancellations
{
public:
    template<typename Context>
    explicit Cancellations(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_executionbook(context)
      , m_factory(context)
    {
        init();
    }

    // Examine order book and generate fills cancellations if necessary
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_expr) {
            cancellations(next);
        }
        return m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(m_orderbook.orders.empty() ? 5 : 1);
    }

private:
    using Order     = Context::Order;
    using Execution = Context::Execution;
    using ordrid    = Context::OrderBook::orderid;
    using execid    = Context::ExecutionBook::execid;

    // Series of fills for a given order size
    struct Cancellation
    {
        unsigned quantity;
        double   price;
    };

    template<typename Next>
    void cancellations(Next& next)
    {
        for (const auto& execution : m_executionbook.executions) {
            if (execution.live()) {
                if (std::find_if(m_cancellations.begin(), m_cancellations.end(), [&](const auto& e) {
                        return execution.quantity() == e.quantity && execution.price() == e.price;
                    }) != m_cancellations.end()) {
                    cancel(execution, next);
                }
            }
        }
    }

    template<typename Next>
    void cancel(const Execution& execution, Next& next)
    {
        auto& idx = m_orderbook.orders.get<ordrid>();
        auto  itr = idx.find(execution.orderID());
        if (itr == idx.end()) {
            std::string error = "Unable to find order for client order ID: " + execution.orderID();
            log << level::error << ts << here << ' ' << error << std::endl;
            return;
        }
        log << level::error << ts << here << ' ' << "Cancelling " << execution << std::endl;
        return cancel(execution, *itr, next);
    }

    template<typename Next>
    void cancel(const Execution& execution, const Order& order, Next& next)
    {
        execution.cancel();
        next.put(m_factory.report(execution, order));
    }

    void init()
    {
        static const auto& cancellations = m_executionbook.config()["ExecutionBook.Cancellations"];

        for (int i = 1; i <= cancellations.size(); ++i) {
            const auto& cancellation = cancellations[i];

            unsigned qty = cancellation["Quantity"];
            double   prx = cancellation["Price"];
            log << level::debug << ts << here << ' ' << qty << '@' << prx << std::endl;
            m_cancellations.push_back({ qty, prx });
        }
    }

    // Cancellation parameters
    std::vector<Cancellation> m_cancellations;

    Context::Session&       m_session;
    Context::OrderBook&     m_orderbook;
    Context::ExecutionBook& m_executionbook;
    Factory                 m_factory;

    timepoint m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(1);
};

} // namespace Fix::Modules::ExecutionBook
#endif
