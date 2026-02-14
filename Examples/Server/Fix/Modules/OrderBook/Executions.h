#ifndef Modules_OrderBook_Executions_dot_h
#define Modules_OrderBook_Executions_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context/Session/Session.h"
#include "Context/OrderBook/OrderBook.h"
#include "Context/ExecutionBook/ExecutionBook.h"

#include <quickfix/Message.h>

namespace Fix::Modules::OrderBook {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Executions
{
public:
    template<typename Context>
    explicit Executions(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_executionbook(context)
      , m_factory(context)
    {
        init();
    }

    // Examine order book and generate fills cancells and corrections if necessary
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_expr) {
            fills(next);
        }
        return m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(m_orderbook.orders.empty() ? 5 : 1);
    }

private:
    using Order     = Context::Order;
    using Execution = Context::Execution;

    // Series of fills for a given order size
    struct Fill
    {
        unsigned quantity;
        double   price;
    };
    using Fills = std::vector<Fill>;

    template<typename Next>
    void fills(Next& next)
    {
        for (const auto& order : m_orderbook.orders) {
            if (auto itr = m_fills.find(order.quantity()); itr != m_fills.end()) {
                if (order.status() != Order::Status::Filled && order.status() != Order::Status::PartiallyFilled) {
                    log << level::debug << vmark << "Generating fills for order " << order.clientOrderID() << std::endl;
                    fill(order, itr->second, next);
                }
            }
        }
    }

    template<typename Next>
    void fill(const Order& order, const Fills& fills, Next& next)
    {
        for (const auto& f : fills) {
            fill(order, f, next);
        }
    }

    template<typename Next>
    void fill(const Order& order, const Fill& fill, Next& next)
    {
        Execution exec = Execution(order, fill.quantity, fill.price);
        if (!m_executionbook.executions.insert(exec).second) {
            std::string error = "Unable to record the execution for order: " + order.clientOrderID();
            log << level::error << _here_ << ' ' << error << std::endl;
            throw std::runtime_error(error);
        }
        log << level::info << _here_ << ' ' << "Inserted execution:" << exec << " for order:" << order << std::endl;
        next.put(m_factory.execution(exec, order));
    }

    void init()
    {
        static const auto& executions = m_executionbook.config()["OrderBook.Executions"];

        for (int i = 1; i <= executions.size(); ++i) {
            const auto& execution = executions[i];
            int         quantity  = execution["Quantity"];

            const sol::table& fills = execution["Fills"];
            for (int f = 1; f <= fills.size(); ++f) {
                const auto& fill = fills[f];

                unsigned qty = fill["Quantity"];
                double   prx = fill["Price"];
                log << level::debug << _here_ << ' ' << quantity << ' ' << qty << '@' << prx << std::endl;
                m_fills[quantity].push_back({ qty, prx });
            }
        }
    }

    // Fill parameters
    std::map<unsigned, Fills> m_fills;

    // Cancellation parameters
    Fills m_cancells;

    Context::Session&       m_session;
    Context::OrderBook&     m_orderbook;
    Context::ExecutionBook& m_executionbook;
    Factory                 m_factory;

    timepoint m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(1);
};

} // namespace Fix::Modules::OrderBook
#endif
