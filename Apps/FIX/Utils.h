#ifndef FIX_Utils_dot_h
#define FIX_Utils_dot_h

#include <charconv>
#include <ostream>
#include <iostream>

namespace FIX {
namespace detail {

class fixdump_wrapper
{
    const char* m_buf;
    std::size_t m_len;
    char        m_sep;

    template<class CharT, class Traits>
    void print(std::basic_ostream<CharT, Traits>& os) const
    {
        for (std::size_t i = 0; i < m_len; ++i) {
            os << (m_buf[i] == 0x01 || m_buf[i] == 0x00 ? '|' : m_buf[i]);
        }
    }

public:
    fixdump_wrapper(const char* buf, std::size_t len, char sep)
      : m_buf(buf)
      , m_len(len)
      , m_sep(sep)
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const fixdump_wrapper& x)
    {
        x.print(os);
        return os;
    }
};

} // namespace detail

inline detail::fixdump_wrapper
fixdump(const void* buf, std::size_t bsize, char sep = '|')
{
    return { reinterpret_cast<const char*>(buf), bsize, sep };
}

inline detail::fixdump_wrapper
fixdump(const std::string buf, char sep = '|')
{
    return { buf.data(), buf.size(), sep };
}

inline detail::fixdump_wrapper
fixdump(const std::string_view buf, char sep = '|')
{
    return { buf.data(), buf.size(), sep };
}

template<std::integral T = int>
T
to_int(std::string_view str, T defval = T()) noexcept
{
    if (str.empty()) {
        return {};
    }

    T value;
    const auto [ptr, ec]{ std::from_chars(str.begin(), str.end(), value) };
    if (ec != std::errc() || ptr != str.end()) {
        return {};
    }
    return value;
}

template<std::floating_point T = double>
T
to_float(std::string_view str, T defval = T()) noexcept
{
    if (str.empty()) {
        return defval;
    }

    T value;
    const auto [ptr, ec]{ std::from_chars(str.begin(), str.end(), value) };
    if (ec != std::errc() || ptr != str.end()) {
        return defval;
    }
    return value;
}

inline std::pair<int, int>
split(double price)
{
    int precision = 1000; // TODO - configure precision

    // return { int(std::floor(price)), int(std::floor(precision * (price - std::floor(price)))) };
    return { int(price * precision), -3 };
}

} // namespace FIX

#endif
