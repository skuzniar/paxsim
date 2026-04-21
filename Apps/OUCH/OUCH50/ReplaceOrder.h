#ifndef ReplaceOrder_h
#define ReplaceOrder_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ReplaceOrder
{
    static constexpr char Type = 'U';

    UnsequencedData header = { Type, sizeof(ReplaceOrder) };
    UserRefNum      origUserRefNum;
    UserRefNum      newUserRefNum;
    Int32           quantity;
    Price           price;
    Alpha<6>        user;
    Int16           appendageLength;

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const ReplaceOrder& o)
{
    // clang-format off
    s << "[ReplaceOrder]=";
    s << "Header: "          << o.header          << ' ';
    s << "OrigUserRefNum: "  << o.origUserRefNum  << ' ';
    s << "NewUserRefNum: "   << o.newUserRefNum   << ' ';
    s << "Quantity: "        << o.quantity        << ' ';
    s << "Price: "           << o.price           << ' ';
    s << "User: "            << o.user            << ' ';
    s << "AppendageLength: " << o.appendageLength;
    // clang-format on
    return s;
}

template<typename App>
ReplaceOrder&
operator+=(ReplaceOrder& o, const App& a)
{
    auto* offset = reinterpret_cast<char*>(std::addressof(o)) + o.size();
    new (offset) App(a);
    o.appendageLength += a.size();
    o.header.header.length += a.size();
    return o;
}

} // namespace OUCH::OUCH50

#endif
