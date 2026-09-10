#ifndef AccountQueryResponse_h
#define AccountQueryResponse_h

#include "PacketHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct AccountQueryResponse
{
    static constexpr char Type = 'Q';

    SequencedData header = { Type, sizeof(AccountQueryResponse) };
    Timestamp     timestamp;
    UserRefNum    nextUserRefNum;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const AccountQueryResponse& o)
{
    // clang-format off
    s << "[AccountQueryResponse]=";
    s << "Header: "         << o.header         << ' ';
    s << "Timestamp: "      << o.timestamp      << ' ';
    s << "NextUserRefNum: " << o.nextUserRefNum;
    // clang-format on
    return s;
}

} // namespace BOE::BOE2

#endif
