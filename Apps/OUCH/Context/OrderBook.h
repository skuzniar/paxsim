#ifndef OUCH_Context_OrderBook_dot_h
#define OUCH_Context_OrderBook_dot_h

#include <OUCH/Types.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace OUCH::Context {

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

    using orderid_t = decltype(std::declval<Order>().orderID());
    using clordid_t = decltype(std::declval<Order>().clordID());

    bmi::multi_index_container<
        // clang-format off
        Order,
            bmi::indexed_by <
                bmi::ordered_unique     < bmi::tag<orderid>, bmi::const_mem_fun<Order, orderid_t, &Order::orderID>>,
                bmi::ordered_unique     < bmi::tag<clordid>, bmi::const_mem_fun<Order, orderid_t, &Order::clordID>>
            >
        > orders;
    // clang-format on

    template<typename Config>
    explicit OrderBook(const Config& config)
    {
    }
};

} // namespace OUCH::Context
#endif
