#ifndef ServerHeartbeat_h
#define ServerHeartbeat_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ServerHeartbeat
{
    static constexpr char Type = 'H';

    PacketHeader header = { Type, sizeof(ServerHeartbeat) };

    std::size_t size() const
    {
        return header.size();
    }
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

} // namespace OUCH::OUCH50

#endif
