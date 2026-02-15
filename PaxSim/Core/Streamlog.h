#ifndef PaxSim_Core_Streamlog_dot_h
#define PaxSim_Core_Streamlog_dot_h

#include <ostream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <type_traits>

namespace PaxSim::Core {

//----------------------------------------------------------------------------------------------------------------------
// This is a minimalistic implementation of the logging functionality. It uses std::ostream as the output stream and
// adds some rudimentary log level support.
//----------------------------------------------------------------------------------------------------------------------
namespace detail {

// Helper functions to manage log level
inline int
logidx()
{
    static int i = std::ios_base::xalloc();
    return i;
}

enum class threshold : unsigned int
{
    trace   = 0b00000001,
    debug   = 0b00000010,
    info    = 0b00000100,
    warning = 0b00001000,
    error   = 0b00010000,
    fatal   = 0b00100000
};

enum class level : unsigned int
{
    trace   = 0b00000001,
    debug   = 0b00000011,
    info    = 0b00000111,
    warning = 0b00001111,
    error   = 0b00011111,
    fatal   = 0b00111111
};

// Pack the threshold and level into the iword of the ostream.
union logpack
{
    logpack(unsigned int i)
      : i(i)
    {
        if (i == 0) {
            l.threshold = std::underlying_type_t<enum threshold>(threshold::info);
            l.level     = std::underlying_type_t<enum level>(level::info);
        }
    }

    operator unsigned int() const
    {
        return i;
    }

    threshold get_threshold() const
    {
        return static_cast<enum threshold>(l.threshold);
    }

    level get_level() const
    {
        return static_cast<enum level>(l.level);
    }

    bool on() const
    {
        return (l.threshold & l.level) != 0;
    }

    struct
    {
        unsigned int threshold : 8;
        unsigned int level     : 8;
    } l;
    unsigned int i;
};

template<class CharT, class Traits>
void
set_threshold(std::basic_ostream<CharT, Traits>& os, threshold t)
{
    logpack pack       = os.iword(logidx());
    bool    on         = pack.on();
    pack.l.threshold   = std::underlying_type_t<threshold>(t);
    os.iword(logidx()) = pack;

    if (on == pack.on()) {
        return;
    }

    if (pack.on()) {
        os.clear(os.rdstate() & ~std::ios_base::failbit);
    } else {
        os.setstate(std::ios_base::failbit);
    }
}

template<class CharT, class Traits>
void
set_level(std::basic_ostream<CharT, Traits>& os, level l)
{
    logpack pack       = os.iword(logidx());
    bool    on         = pack.on();
    pack.l.level       = std::underlying_type_t<level>(l);
    os.iword(logidx()) = pack;

    if (on == pack.on()) {
        return;
    }

    if (pack.on()) {
        os.clear(os.rdstate() & ~std::ios_base::failbit);
    } else {
        os.setstate(std::ios_base::failbit);
    }
}

template<class CharT, class Traits>
threshold
get_threshold(std::basic_ostream<CharT, Traits>& os)
{
    logpack   pack = os.iword(logidx());
    threshold retv = pack.get_threshold();
    return retv;
}

template<class CharT, class Traits>
level
get_level(std::basic_ostream<CharT, Traits>& os)
{
    logpack pack = os.iword(logidx());
    level   retv = pack.get_level();
    return retv;
}

} // namespace detail

using threshold = detail::threshold;
using level     = detail::level;

inline std::ostream&
operator<<(std::ostream& s, threshold t)
{
    set_threshold(s, t);
    return s;
}

inline std::ostream&
operator<<(std::ostream& s, level l)
{
    set_level(s, l);
    return s;
}

//----------------------------------------------------------------------------------------------------------------------
// Static instance of the logging stream uses std::clog as the default output stream.
//----------------------------------------------------------------------------------------------------------------------
struct logstream : public std::ostream
{
    explicit logstream(std::ostream& os)
    : std::ostream(os.rdbuf())
    {
    }

    void is(std::ostream& os) {
        rdbuf(os.rdbuf());
    }

    void is(const std::string& file) {
        ofs = std::ofstream(file);
        rdbuf(ofs.rdbuf());
    }

    private:
    std::ofstream ofs;
};

// Must be defined in the user code
extern logstream log;

namespace detail {

template<typename O>
struct byval
{
    O m_o;

    explicit byval(O o)
      : m_o(std::move(o))
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const byval& x)
    {
        return os << x.m_o;
    }
};

template<typename O>
struct byref
{
    const O& m_o;

    explicit byref(const O& o)
      : m_o(o)
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const byref& x)
    {
        return os << x.m_o;
    }
};

template<typename O>
class paintval : byval<O>
{
    const char* m_c;

public:
    paintval(O o, const char* c)
      : byval<O>(std::move(o))
      , m_c(c)
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const paintval& x)
    {
        return os << char(033) << '[' << x.m_c << 'm' << x.m_o << char(033) << '[' << '0' << 'm';
    }
};

template<typename O>
class paintref : byref<O>
{
    const char* m_c;

public:
    paintref(const O& o, const char* c)
      : byref<O>(o)
      , m_c(c)
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const paintref& x)
    {
        return os << char(033) << '[' << x.m_c << 'm' << x.m_o << char(033) << '[' << '0' << 'm';
    }
};

} // namespace detail

// clang-format off
template<typename O> auto     red(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "31");} else {return detail::paintval(std::forward<O>(o), "31");}}
template<typename O> auto   green(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "32");} else {return detail::paintval(std::forward<O>(o), "32");}}
template<typename O> auto  yellow(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "33");} else {return detail::paintval(std::forward<O>(o), "33");}}
template<typename O> auto    blue(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "34");} else {return detail::paintval(std::forward<O>(o), "34");}}
template<typename O> auto magenta(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "35");} else {return detail::paintval(std::forward<O>(o), "35");}}
template<typename O> auto    cyan(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "36");} else {return detail::paintval(std::forward<O>(o), "36");}}
template<typename O> auto   white(O&& o) {if constexpr (std::is_lvalue_reference_v<O&&>) {return detail::paintref(o, "37");} else {return detail::paintval(std::forward<O>(o), "37");}}
// clang-format on

template<class CharT, class Traits>
auto&
ts(std::basic_ostream<CharT, Traits>& os)
{
    std::ios state(nullptr);
    state.copyfmt(os);

    const auto time_point       = std::chrono::system_clock::now();
    const auto time             = std::chrono::system_clock::to_time_t(time_point);
    const auto localtime        = *std::localtime(&time);
    const auto time_since_epoch = time_point.time_since_epoch();
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch).count() % 1000000;

    os << std::put_time(&localtime, "%T") << "." << std::setw(6) << std::setfill('0') << microseconds;
    os.copyfmt(state);

    return os;
};

template<class CharT, class Traits>
auto&
in(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << red(">>> ");
}

template<class CharT, class Traits>
auto&
out(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << green("<<< ");
}

template<class CharT, class Traits>
auto&
status(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << white("*** ");
}

template<class CharT, class Traits>
auto&
vmark(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << yellow("=== ");
}

template<class CharT, class Traits>
auto&
hmark(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << yellow("||| ");
}

template<class CharT, class Traits>
auto&
ctlmark(std::basic_ostream<CharT, Traits>& os)
{
    return os << ts << ' ' << magenta("+++ ");
}

namespace detail {

class hexdump_wrapper
{
    const void* m_buf;
    std::size_t m_len;
    std::size_t m_cpl;

    template<class CharT, class Traits>
    void print(std::basic_ostream<CharT, Traits>& os) const
    {
        const auto* cbuf = reinterpret_cast<const unsigned char*>(m_buf);

        std::ios state(nullptr);
        state.copyfmt(os);

        for (std::size_t i = 0; i < m_len; ++i) {
            if (m_cpl != std::numeric_limits<std::size_t>::max() && i % m_cpl == 0) {
                os << '\n' << std::setw(6) << std::setfill('0') << i << std::dec << ' ';
            }

            int c = cbuf[i];
            if (std::isgraph(c) != 0) {
                os << std::setw(2) << std::setfill(' ') << cbuf[i] << ' ';
            } else if (std::iscntrl(c) != 0) {
                switch (c) {
                    case '\t':
                        os << std::setw(2) << std::setfill(' ') << "\\t" << ' ';
                        break;
                    case '\n':
                        os << std::setw(2) << std::setfill(' ') << "\\n" << ' ';
                        break;
                    case '\v':
                        os << std::setw(2) << std::setfill(' ') << "\\v" << ' ';
                        break;
                    case '\f':
                        os << std::setw(2) << std::setfill(' ') << "\\f" << ' ';
                        break;
                    case '\r':
                        os << std::setw(2) << std::setfill(' ') << "\\r" << ' ';
                        break;
                    case 0x7f:
                        os << std::setw(2) << std::setfill(' ') << "\\b" << ' ';
                        break;
                    default:
                        os << std::setw(2) << std::setfill('0') << std::hex << c << std::dec << ' ';
                        break;
                }
            } else if (cbuf[i] == ' ') {
                os << std::setw(2) << "' '" << ' ';
            } else {
                os << std::setw(2) << std::setfill('0') << std::hex << c << std::dec << ' ';
            }
        }
        os.copyfmt(state);
    }

public:
    hexdump_wrapper(const void* buf,
                    std::size_t len,
                    std::size_t chars_per_line = std::numeric_limits<std::size_t>::max())
      : m_buf(buf)
      , m_len(len)
      , m_cpl(chars_per_line)
    {
    }

    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         const hexdump_wrapper&             x)
    {
        x.print(os);
        return os;
    }
};

} // namespace detail

inline detail::hexdump_wrapper
hexdump(const void* buf, std::size_t len, std::size_t chars_per_line = std::numeric_limits<std::size_t>::max())
{
    return { buf, len, chars_per_line };
}

inline detail::hexdump_wrapper
hexdump(const std::string buf, std::size_t chars_per_line = std::numeric_limits<std::size_t>::max())
{
    return { buf.data(), buf.size(), chars_per_line };
}

inline detail::hexdump_wrapper
hexdump(const std::string_view buf, std::size_t chars_per_line = std::numeric_limits<std::size_t>::max())
{
    return { buf.data(), buf.size(), chars_per_line };
}

inline detail::hexdump_wrapper
hexdumpx(const void* buf, std::size_t len)
{
    return { buf, len, 10 };
}

//----------------------------------------------------------------------------------------------------------------------
// File without path.
//----------------------------------------------------------------------------------------------------------------------
namespace detail {

constexpr bool
is_path_sep(char c)
{
    return c == '/' || c == '\\';
}

constexpr const char*
strip_path(const char* path)
{
    const auto* lastname = path;
    for (const auto* p = path; *p != 0; ++p) {
        if (is_path_sep(*p) && (*(p + 1) != 0))
            lastname = p + 1;
    }
    return lastname;
}

} // namespace detail

// The filename with extension but no path
#define _file_ PaxSim::Core::detail::strip_path(__FILE__)
#define _line_ __LINE__

#define here ' ' << _file_ << ':' << _line_

} // namespace PaxSim::Core

#endif
