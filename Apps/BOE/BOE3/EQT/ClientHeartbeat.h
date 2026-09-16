#ifndef ClientHeartbeat_h
#define ClientHeartbeat_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct ClientHeartbeat
{
    enum
    {
        Type = 3
    };

    Header header = { Type, sizeof(ClientHeartbeat) };
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const ClientHeartbeat& o)
{
    // clang-format off
    s << "[ClientHeartbeat]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace boe::boe3::eqt

#endif
