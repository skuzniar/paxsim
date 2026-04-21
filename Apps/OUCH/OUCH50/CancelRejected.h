#ifndef CancelRejected_h
#define CancelRejected_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct CancelRejected
{
    static constexpr char Type = 'I';

    SequencedData header = { Type, sizeof(CancelRejected) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    RejectReason  reason;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const CancelRejected& o)
{
    // clang-format off
    s << "[CancelRejected]=";
    s << "Header: "     << o.header     << ' ';
    s << "Timestamp: "  << o.timestamp  << ' ';
    s << "UserRefNum: " << o.userRefNum << ' ';
    s << "Reason: "     << o.reason;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
