#ifndef Utils_dot_h
#define Utils_dot_h

#include <ostream>
#include <iostream>

namespace Fix {
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

} // namespace Fix

#endif
