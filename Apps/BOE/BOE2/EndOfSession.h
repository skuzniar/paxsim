#ifndef EndOfSession_h
#define EndOfSession_h

#include "PacketHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct EndOfSession
{
    static constexpr char Type = 'Z';

    PacketHeader header = { Type, sizeof(EndOfSession) };
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const EndOfSession& o)
{
    // clang-format off
    s << "[EndOfSession]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace BOE::BOE2

#endif
