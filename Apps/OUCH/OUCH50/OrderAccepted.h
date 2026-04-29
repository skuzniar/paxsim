#ifndef OrderAccepted_h
#define OrderAccepted_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderAccepted
{
    static constexpr char Type = 'A';

    SequencedData      header = { Type, sizeof(OrderAccepted) };
    Timestamp          timestamp;
    UserRefNum         userRefNum;
    Price              price;
    UInt64             orderReferenceNumber;
    BuySellIndicator   buySellIndicator;
    UInt32             orderBook;
    UInt32             quantity;
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
operator<<(std::ostream& s, const OrderAccepted& o)
{
    // clang-format off
    s << "[OrderAccepted]=";
    s << "Header: "                       << o.header                       << ' ';
    s << "Timestamp: "                    << o.timestamp                    << ' ';
    s << "UserRefNum: "                   << o.userRefNum                   << ' ';
    s << "Price: "                        << o.price                        << ' ';
    s << "OrderReferenceNumber: "         << o.orderReferenceNumber         << ' ';
    s << "BuySellIndicator: "             << o.buySellIndicator             << ' ';
    s << "OrderBook: "                    << o.orderBook                    << ' ';
    s << "Quantity: "                     << o.quantity                     << ' ';
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

} // namespace OUCH::OUCH50

#endif
