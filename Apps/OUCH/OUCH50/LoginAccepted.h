#ifndef LoginAccepted_h
#define LoginAccepted_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct LoginAccepted
{
    static constexpr char Type = 'A';

    PacketHeader header = { Type, sizeof(LoginAccepted) };
    Alpha<10>    session;
    Alpha<20>    sequenceNumber;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginAccepted& o)
{
    // clang-format off
    s << "[LoginAccepted]=";
    s << "Header: "         << o.header         << ' ';
    s << "Session: "        << o.session        << ' ';
    s << "SequenceNumber: " << o.sequenceNumber;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
