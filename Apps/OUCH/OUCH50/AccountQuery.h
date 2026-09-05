#ifndef AccountQuery_h
#define AccountQuery_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct AccountQuery
{
    static constexpr char Type = 'Q';

    UnsequencedData header = { Type, sizeof(AccountQuery) };
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const AccountQuery& o)
{
    // clang-format off
    s << "[AccountQuery]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
