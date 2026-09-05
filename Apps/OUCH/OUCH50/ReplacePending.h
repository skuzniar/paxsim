#ifndef ReplacePending_h
#define ReplacePending_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ReplacePending
{
    static constexpr char Type = 'N';

    SequencedData header = { Type, sizeof(ReplacePending) };
    Timestamp     timestamp;
    UserRefNum    origUserRefNum;
    UserRefNum    newUserRefNum;
    PendingReason reason;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const ReplacePending& o)
{
    // clang-format off
    s << "[ReplacePending]=";
    s << "Header: "         << o.header         << ' ';
    s << "Timestamp: "      << o.timestamp      << ' ';
    s << "OrigUserRefNum: " << o.origUserRefNum << ' ';
    s << "NewUserRefNum: "  << o.newUserRefNum  << ' ';
    s << "Reason: "         << o.reason;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
