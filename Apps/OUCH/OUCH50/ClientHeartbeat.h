#ifndef ClientHeartbeat_h
#define ClientHeartbeat_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ClientHeartbeat
{
    static constexpr char Type = 'R';

    PacketHeader header = { Type, sizeof(ClientHeartbeat) };
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

} // namespace OUCH::OUCH50

#endif
