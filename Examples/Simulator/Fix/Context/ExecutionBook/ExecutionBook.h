#ifndef Context_ExecutionBook_ExecutionBook_dot_h
#define Context_ExecutionBook_ExecutionBook_dot_h

#include "Execution.h"
#include "Config.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace Fix::Context {

namespace bmi = boost::multi_index;

struct ExecutionBook
{
    struct execid
    {};
    struct ordrid
    {};

    // Collection of executions
    bmi::multi_index_container<
        // clang-format off
        Execution,
            bmi::indexed_by <
                bmi::ordered_unique     < bmi::tag<execid>, bmi::const_mem_fun<Execution, Execution::ID,      &Execution::executionID>>,
                bmi::ordered_non_unique < bmi::tag<ordrid>, bmi::const_mem_fun<Execution, Execution::OrderID, &Execution::orderID>>
            >
        > executions;
    // clang-format on

    template<typename Config>
    explicit ExecutionBook(const Config& config)
      : m_config(config)
    {
    }

    const auto& config() const
    {
        return m_config;
    }

    // Compute the total fills for the order
    [[nodiscard]] std::pair<unsigned, double> fills(const Order& order) const
    {
        const auto& oidx = executions.template get<ordrid>();

        unsigned qty = 0;
        double   avg = 0;

        auto range = oidx.equal_range(order.orderID());
        for (auto itr = range.first; itr != range.second; ++itr) {
            if (itr->live()) {
                qty += itr->quantity();
                avg += itr->quantity() * itr->price();
            }
        }
        return { qty, avg > 0 ? avg / qty : 0 };
    }

private:
    const Config& m_config;
};

} // namespace Fix::Context
#endif
