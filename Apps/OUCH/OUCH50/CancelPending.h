#ifndef CancelPending_h
#define CancelPending_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct CancelPending
{
    static constexpr char Type = 'P';

    SequencedData header = { Type, sizeof(CancelPending) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    PendingReason reason;
    // Appendages;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const CancelPending& o)
{
    // clang-format off
    s << "[CancelPending]=";
    s << "Header: "     << o.header     << ' ';
    s << "Timestamp: "  << o.timestamp  << ' ';
    s << "UserRefNum: " << o.userRefNum << ' ';
    s << "Reason: "     << o.reason;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
