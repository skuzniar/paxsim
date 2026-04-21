#ifndef OUCH_Utils_dot_h
#define OUCH_Utils_dot_h

#include <algorithm>
#include <string>
#include <chrono>
#include <cstddef>

namespace OUCH {

//---------------------------------------------------------------------------------------------------------------------
// Utilities to fill fixed size characted arrays
//---------------------------------------------------------------------------------------------------------------------
template<typename M>
inline size_t
ritoa(char* target, size_t tsize, M number, char f = ' ')
{
    auto impl = [=](M num, char sign, char fill) mutable {
        if (tsize > 0) {
            do {
                auto rem        = num % 10L;
                target[--tsize] = static_cast<char>('0' + rem);
                num             = num / 10L;
            } while (tsize > 0 && num != 0);

            if (tsize > 0 && sign != fill) {
                target[--tsize] = sign;
            }
            std::fill(target, target + tsize, fill);
        }
        return tsize;
    };
    return number < 0 ? impl(-number, '-', f) : impl(number, f, f);
}

inline std::string
to_utcstring(const struct timeval& tv, unsigned precision = 6)
{
    char buffer[64];

    if (const auto* tvp = std::gmtime(&tv.tv_sec); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.06d", tv.tv_usec);
            if (precision > 6) {
                std::fill(buffer + count + 1 + 6, buffer + count + 1 + precision, '0');
            }
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
xto_utcstring(const struct timeval& tv, unsigned precision)
{
    char buffer[64];

    if (const auto* tvp = std::gmtime(&tv.tv_sec); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.6d", tv.tv_usec);
            if (precision > 6) {
                std::fill(buffer + count + 1 + 6, buffer + count + 1 + precision, '0');
            }
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
to_utcstring(std::chrono::seconds s, unsigned precision)
{
    char buffer[64];

    std::time_t t = s.count();

    if (const auto* tvp = std::gmtime(&t); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.03d", 0);
            if (precision > 3) {
                std::fill(buffer + count + 1 + 3, buffer + count + 1 + precision, '0');
            }
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }

    return buffer;
}

inline std::string
to_utcstring(std::chrono::milliseconds m, unsigned precision)
{
    char buffer[64];

    auto s = std::chrono::duration_cast<std::chrono::seconds>(m);
    m -= std::chrono::duration_cast<std::chrono::milliseconds>(s);
    std::time_t t = s.count();

    if (const auto* tvp = std::gmtime(&t); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.3lld", m.count());
            if (precision > 3) {
                std::fill(buffer + count + 1 + 3, buffer + count + 1 + precision, '0');
            }
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }

    return buffer;
}

inline std::string
to_utcstring(std::chrono::microseconds u, unsigned precision)
{
    char buffer[64];

    auto s = std::chrono::duration_cast<std::chrono::seconds>(u);
    u -= std::chrono::duration_cast<std::chrono::microseconds>(s);
    std::time_t t = s.count();

    if (const auto* tvp = std::gmtime(&t); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.6lld", u.count());
            if (precision > 6) {
                std::fill(buffer + count + 1 + 6, buffer + count + 1 + precision, '0');
            }
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
to_utcstring(std::chrono::nanoseconds n, unsigned precision)
{
    char buffer[64];

    auto s = std::chrono::duration_cast<std::chrono::seconds>(n);
    n -= std::chrono::duration_cast<std::chrono::nanoseconds>(s);
    std::time_t t = s.count();

    if (const auto* tvp = std::gmtime(&t); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%Y%m%d-%T", tvp);

        precision = std::min(precision, 9U);
        if (precision > 0) {
            std::snprintf(buffer + count, sizeof(buffer) - count, ".%.9lld", n.count());
            buffer[count + 1 + precision] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
to_utcymd(const struct timeval& tv)
{
    char buffer[64];
    if (const auto* tvp = std::gmtime(&tv.tv_sec); tvp != nullptr) {
        auto count    = std::strftime(buffer, sizeof(buffer), "%Y%m%d", tvp);
        buffer[count] = '\0';
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
to_utchmsu(const struct timeval& tv)
{
    char buffer[64];
    if (const auto* tvp = std::gmtime(&tv.tv_sec); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%T", tvp);
        std::snprintf(buffer + count, sizeof(buffer) - count, ".%.6d", tv.tv_usec);
        buffer[count + 1 + 6] = '\0';
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

inline std::string
to_utchmsn(std::chrono::nanoseconds n)
{
    char buffer[64];

    auto s = std::chrono::duration_cast<std::chrono::seconds>(n);
    n -= std::chrono::duration_cast<std::chrono::nanoseconds>(s);
    std::time_t t = s.count();

    if (const auto* tvp = std::gmtime(&t); tvp != nullptr) {
        auto count = std::strftime(buffer, sizeof(buffer), "%T", tvp);
        std::snprintf(buffer + count, sizeof(buffer) - count, ".%.9lld", n.count());
        buffer[count + 1 + 9] = '\0';
    } else {
        buffer[0] = '\0';
    }
    return buffer;
}

} // namespace OUCH

#endif
