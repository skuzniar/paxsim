#ifndef Context_OrderBook_OrderBook_dot_h
#define Context_OrderBook_OrderBook_dot_h

#include "Order.h"
#include "Config.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace Fix::Context {

//---------------------------------------------------------------------------------------------------------------------
// Order book. Collection of orders
//---------------------------------------------------------------------------------------------------------------------
namespace bmi = boost::multi_index;

struct OrderBook
{
    struct orderid
    {};
    struct clordid
    {};

    bmi::multi_index_container<
        // clang-format off
        Order,
            bmi::indexed_by <
                bmi::ordered_unique     < bmi::tag<orderid>, bmi::const_mem_fun<Order, Order::ID, &Order::orderID>>,
                bmi::ordered_unique     < bmi::tag<clordid>, bmi::const_mem_fun<Order, Order::ID, &Order::clientOrderID>>
            >
        > orders;
    // clang-format on

    template<typename Config>
    explicit OrderBook(const Config& config)
      : m_config(config)
    {
    }

    const auto& config() const
    {
        return m_config;
    }

private:
    const Config& m_config;
};

} // namespace Fix::Context
#endif
