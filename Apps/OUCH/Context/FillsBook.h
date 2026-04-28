#ifndef OUCH_Context_FillsBook_dot_h
#define OUCH_Context_FillsBook_dot_h

#include <OUCH/Types.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace OUCH::Context {

namespace bmi = boost::multi_index;

struct FillsBook
{
    struct orderid
    {};

    using orderid_t = decltype(std::declval<Fill>().orderID());

    // Collection of fills
    bmi::multi_index_container<
        // clang-format off
        Fill,
            bmi::indexed_by <
                bmi::ordered_non_unique < bmi::tag<orderid>, bmi::const_mem_fun<Fill, orderid_t, &Fill::orderID>>
            >
        > fills;
    // clang-format on

    template<typename Config>
    explicit FillsBook(const Config& config)
    {
    }

    // Compute the total fills for the order
    [[nodiscard]] std::pair<unsigned, double> cumulative(orderid_t id) const
    {
        const auto& oidx = fills.template get<orderid>();

        unsigned qty = 0;
        double   avg = 0;

        auto range = oidx.equal_range(id);
        for (auto itr = range.first; itr != range.second; ++itr) {
            if (itr->live()) {
                qty += itr->quantity();
                avg += itr->quantity() * itr->price();
            }
        }
        return { qty, avg > 0 ? avg / qty : 0 };
    }
};

} // namespace OUCH::Context
#endif
