#ifndef EnterOrder_h
#define EnterOrder_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct EnterOrder
{
    static constexpr char Type = 'O';

    UnsequencedData    header     = { Type, sizeof(EnterOrder) };
    UserRefNum         userRefNum = 1;
    BuySellIndicator   buySellIndicator;
    UInt32             quantity;
    UInt32             orderBook;
    Price              price;
    Alpha<6>           user;
    UInt32             executionWithinFirm;
    UInt32             investmentDecisionWithinFirm;
    UInt32             clientIdentifier;
    PartyRoleQualifier partyRoleQualifier;
    Capacity           capacity;
    AlgoIndicator      algoIndicator;
    UInt16             appendageLength;

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const EnterOrder& o)
{
    // clang-format off
    s << "[EnterOrder]=";
    s << "Header: "                       << o.header                       << ' ';
    s << "UserRefNum: "                   << o.userRefNum                   << ' ';
    s << "BuySellIndicator: "             << o.buySellIndicator             << ' ';
    s << "Quantity: "                     << o.quantity                     << ' ';
    s << "OrderBook: "                    << o.orderBook                    << ' ';
    s << "Price: "                        << o.price                        << ' ';
    s << "User: "                         << o.user                         << ' ';
    s << "ExecutionWithinFirm: "          << o.executionWithinFirm          << ' ';
    s << "InvestmentDecisionWithinFirm: " << o.investmentDecisionWithinFirm << ' ';
    s << "ClientIdentifier: "             << o.clientIdentifier             << ' ';
    s << "PartyRoleQualifier: "           << o.partyRoleQualifier           << ' ';
    s << "Capacity: "                     << o.capacity                     << ' ';
    s << "AlgoIndicator: "                << o.algoIndicator                << ' ';
    s << "AppendageLength: "              << o.appendageLength;
    // clang-format on
    return s;
}

template<typename App>
EnterOrder&
operator+=(EnterOrder& o, const App& a)
{
    auto* offset = reinterpret_cast<char*>(std::addressof(o)) + o.size();
    new (offset) App(a);
    o.appendageLength += a.size();
    o.header.header.length += a.size();
    return o;
}

} // namespace OUCH::OUCH50

#endif
