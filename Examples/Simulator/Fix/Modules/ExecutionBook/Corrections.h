#ifndef Modules_ExecutionBook_Corrections_dot_h
#define Modules_ExecutionBook_Corrections_dot_h

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
class Corrections
{
public:
    template<typename Context>
    explicit Corrections(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_executionbook(context)
      , m_factory(context)
    {
        init();
    }

    // Examine order book and generate fill corrections if necessary
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_expr) {
            corrections(next);
        }
        return m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(m_orderbook.orders.empty() ? 5 : 1);
    }

private:
    using Order     = Context::Order;
    using Execution = Context::Execution;
    using ordrid    = Context::OrderBook::orderid;
    using execid    = Context::ExecutionBook::execid;

    // Series of fills for a given order size
    struct Correction
    {
        unsigned quantity;
        double   price;
        unsigned newQuantity;
        double   newPrice;
    };

    template<typename Next>
    void corrections(Next& next)
    {
        for (const auto& execution : m_executionbook.executions) {
            if (execution.live()) {
                if (auto itr = std::find_if(m_corrections.begin(),
                                            m_corrections.end(),
                                            [&](const auto& c) { return execution.quantity() == c.quantity && execution.price() == c.price; });
                    itr != m_corrections.end()) {
                    correct(execution, *itr, next);
                }
            }
        }
    }

    template<typename Next>
    void correct(const Execution& execution, const Correction& correction, Next& next)
    {
        auto& idx = m_orderbook.orders.get<ordrid>();
        auto  itr = idx.find(execution.orderID());
        if (itr == idx.end()) {
            std::string error = "Unable to find order for client order ID: " + execution.orderID();
            log << level::error << _here_ << ' ' << error << std::endl;
            return;
        }
        log << level::error << _here_ << ' ' << "Correcting " << execution << std::endl;
        return correct(execution, correction, *itr, next);
    }

    template<typename Next>
    void correct(const Execution& execution, const Correction& correction, const Order& order, Next& next)
    {
        execution.m_quantity = correction.newQuantity;
        execution.m_price    = correction.newPrice;
        next.put(m_factory.report(execution, order));
    }

    void init()
    {
        static const auto& corrections = m_executionbook.config()["ExecutionBook.Corrections"];

        for (int i = 1; i <= corrections.size(); ++i) {
            const auto& correction = corrections[i];

            unsigned oldqty = correction["Quantity"];
            double   oldprx = correction["Price"];
            unsigned newqty = correction["NewQuantity"];
            double   newprx = correction["NewPrice"];
            log << level::debug << _here_ << ' ' << oldqty << '@' << oldprx << std::endl;
            m_corrections.push_back({ oldqty, oldprx, newqty, newprx });
        }
    }

    // Cancellation parameters
    std::vector<Correction> m_corrections;

    Context::Session&       m_session;
    Context::OrderBook&     m_orderbook;
    Context::ExecutionBook& m_executionbook;
    Factory                 m_factory;

    timepoint m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(1);
};

} // namespace Fix::Modules::ExecutionBook
#endif
