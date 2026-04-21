#ifndef OrderRejected_h
#define OrderRejected_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderRejected
{
    static constexpr char Type = 'J';

    SequencedData header = { Type, sizeof(OrderRejected) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    RejectReason  reason;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const OrderRejected& o)
{
    // clang-format off
    s << "[OrderRejected]=";
    s << "Header: "     << o.header     << ' ';
    s << "Timestamp: "  << o.timestamp  << ' ';
    s << "UserRefNum: " << o.userRefNum << ' ';
    s << "Reason: "     << o.reason;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
