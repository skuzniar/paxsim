#ifndef CancelOrder_h
#define CancelOrder_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct CancelOrder
{
    static constexpr char Type = 'X';

    UnsequencedData header = { Type, sizeof(CancelOrder) };
    UserRefNum      userRefNum;
    UInt32          quantity;
    Alpha<6>        user;

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const CancelOrder& o)
{
    // clang-format off
    s << "[CancelOrder]=";
    s << "Header: "     << o.header     << ' ';
    s << "UserRefNum: " << o.userRefNum << ' ';
    s << "Quantity: "   << o.quantity   << ' ';
    s << "User: "       << o.user;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
