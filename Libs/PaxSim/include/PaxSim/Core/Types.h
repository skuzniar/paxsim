#ifndef PaxSim_Core_types_dot_h
#define PaxSim_Core_types_dot_h

#include <utility>
#include <chrono>
#include <iostream>
#include <string_view>

namespace PaxSim::Core {

//----------------------------------------------------------------------------------------------------------------------
// Tri-bool
//----------------------------------------------------------------------------------------------------------------------
enum class Result : uint8_t
{
    True,
    False,
    Maybe
};

//----------------------------------------------------------------------------------------------------------------------
// Common timepoint definition
//----------------------------------------------------------------------------------------------------------------------
using timepoint = std::chrono::time_point<std::chrono::steady_clock>;

//----------------------------------------------------------------------------------------------------------------------
// Type aggregate
//---------------------------------------------------------------------------------------------------------------------
template<typename... Parts>
class Aggregate : public Parts...
{
public:
    Aggregate() = default;

    template<typename... Args>
    explicit Aggregate(Args&&... args)
      : Parts(std::forward<Args>(args)...)...
    {
    }
};

//----------------------------------------------------------------------------------------------------------------------
// Message processing module feature detection
//----------------------------------------------------------------------------------------------------------------------
template<typename T, typename R, typename... Args>
concept has_generic = requires(R (T::*m)(Args...)) { m = &T::generic; };

template<typename T, typename... Args>
concept has_init = requires(void (T::*m)(std::add_lvalue_reference_t<Args>...)) { m = &T::init; };

template<typename T, typename... Args>
concept has_eval = requires(void (T::*m)(std::add_lvalue_reference_t<Args>...)) { m = &T::eval; };

template<typename T, typename... Args>
concept has_timeout = requires(timepoint (T::*m)(timepoint, std::add_lvalue_reference_t<Args>...)) { m = &T::timeout; };

template<typename T, typename... Args>
concept has_put_generic = requires(bool (T::*m)(Args...)) { m = &T::put; };

template<typename T, typename M, typename N>
// clang-format off
concept has_put = has_put_generic<T, M,                                                std::add_lvalue_reference_t<N>> ||
                  has_put_generic<T, std::add_lvalue_reference_t<M>,                   std::add_lvalue_reference_t<N>> ||
                  has_put_generic<T, std::add_lvalue_reference_t<std::add_const_t<M>>, std::add_lvalue_reference_t<N>>;
// clang-format on

} // namespace PaxSim::Core

#endif
