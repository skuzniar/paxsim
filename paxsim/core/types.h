#ifndef Paxsim_Core_types_dot_h
#define Paxsim_Core_types_dot_h

#include <chrono>
#include <optional>

namespace paxsim::core {

//----------------------------------------------------------------------------------------------------------------------
// Common timepoint definition
//----------------------------------------------------------------------------------------------------------------------
using timepoint = std::chrono::time_point<std::chrono::steady_clock>;

namespace tag {

//----------------------------------------------------------------------------------------------------------------------
// Empty tag. And syntactically valid constructor.
//----------------------------------------------------------------------------------------------------------------------
class Empty
{
public:
    template<typename... Args>
    Empty(Args... args)
    {
    }
};

//----------------------------------------------------------------------------------------------------------------------
// Pipeline tag. Framework modules should signal they are pipelines by inheriting from this class.
//----------------------------------------------------------------------------------------------------------------------
class Pipeline
{};

template<typename T>
static constexpr bool
is_pipeline()
{
    return std::is_base_of_v<Pipeline, T>;
}

//----------------------------------------------------------------------------------------------------------------------
// Cyclical tag. Modules that wish to process timeout events should signal it by inheriting from this class.
//----------------------------------------------------------------------------------------------------------------------
class Cyclical
{};

template<typename T>
static constexpr bool
is_cyclical()
{
    return std::is_base_of_v<Cyclical, T>;
}

//----------------------------------------------------------------------------------------------------------------------
// Control tag. Modules that wish to process control commands should signal it by inheriting from this class.
//----------------------------------------------------------------------------------------------------------------------
class Control
{};

template<typename T>
static constexpr bool
is_control()
{
    return std::is_base_of_v<Control, T>;
}

} // namespace tag

//----------------------------------------------------------------------------------------------------------------------
// Turn optional into an iretable
//----------------------------------------------------------------------------------------------------------------------
namespace detail {

template<typename T>
struct opt_iterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;

    explicit opt_iterator(pointer ptr)
      : m_ptr(ptr)
    {
    }

    reference operator*() const
    {
        return *m_ptr;
    }
    pointer operator->()
    {
        return m_ptr;
    }

    opt_iterator& operator++()
    {
        m_ptr = nullptr;
        return *this;
    }

    opt_iterator operator++(int)
    {
        opt_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const opt_iterator& a, const opt_iterator& b)
    {
        return a.m_ptr == b.m_ptr;
    };
    friend bool operator!=(const opt_iterator& a, const opt_iterator& b)
    {
        return a.m_ptr != b.m_ptr;
    };

private:
    pointer m_ptr;
};

} // namespace detail

template<typename T>
detail::opt_iterator<T>
begin(std::optional<T>& opt)
{
    return detail::opt_iterator(opt ? &*opt : nullptr);
}

template<typename T>
detail::opt_iterator<T>
end(std::optional<T>&)
{
    return detail::opt_iterator(static_cast<T*>(nullptr));
}

} // namespace paxsim::core

#endif
