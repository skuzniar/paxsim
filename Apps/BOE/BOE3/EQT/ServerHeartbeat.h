#ifndef ServerHeartbeat_h
#define ServerHeartbeat_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct ServerHeartbeat
{
    enum
    {
        Type = 504
    };

    Header header = { Type, sizeof(ServerHeartbeat) };
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const ServerHeartbeat& o)
{
    // clang-format off
    s << "[ServerHeartbeat]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace boe::boe3::eqt

#endif
