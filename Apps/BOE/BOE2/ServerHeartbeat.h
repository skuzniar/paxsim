#ifndef ServerHeartbeat_h
#define ServerHeartbeat_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct ServerHeartbeat
{
    static constexpr MessageType Type = MessageType::ServerHeartbeat;

    MessageHeader header = { Type, sizeof(ServerHeartbeat) };
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

} // namespace BOE::BOE2

#endif
