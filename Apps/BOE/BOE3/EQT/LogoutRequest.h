#ifndef LogoutRequest_h
#define LogoutRequest_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct LogoutRequest
{
    enum
    {
        Type = 2
    };

    Header header = { Type, sizeof(LogoutRequest) };

    LogoutRequest() = default;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LogoutRequest& o)
{
    // clang-format off
    s << "[LogoutRequest]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace boe::boe3::eqt

#endif
