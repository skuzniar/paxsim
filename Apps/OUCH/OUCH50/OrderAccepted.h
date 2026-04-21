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
    Int64              orderReferenceNumber;
    BuySellIndicator   buySellIndicator;
    Int32              orderBook;
    Int32              quantity;
    Alpha<6>           user;
    Int32              executionWithinFirm;
    Int32              investmentDecisionWithinFirm;
    Int32              clientIdentifier;
    PartyRoleQualifier partyRoleQualifier;
    Capacity           capacity;
    AlgoIndicator      algoIndicator;
    Int16              appendageLength;

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
