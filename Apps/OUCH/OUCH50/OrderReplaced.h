#ifndef OrderReplaced_h
#define OrderReplaced_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderReplaced
{
    static constexpr char Type = 'U';

    SequencedData    header = { Type, sizeof(OrderReplaced) };
    Timestamp        timestamp;
    UserRefNum       origUserRefNum;
    UserRefNum       newUserRefNum;
    Price            price;
    Int64            orderReferenceNumber;
    BuySellIndicator buySellIndicator;
    Int32            orderBook;
    UInt32           quantity;
    Alpha<6>         user;
    Int16            appendageLength;

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const OrderReplaced& o)
{
    // clang-format off
    s << "[OrderReplaced]=";
    s << "Header: "               << o.header               << ' ';
    s << "Timestamp: "            << o.timestamp            << ' ';
    s << "OrigUserRefNum: "       << o.origUserRefNum       << ' ';
    s << "NewUserRefNum: "        << o.newUserRefNum        << ' ';
    s << "Price: "                << o.price                << ' ';
    s << "OrderReferenceNumber: " << o.orderReferenceNumber << ' ';
    s << "BuySellIndicator: "     << o.buySellIndicator     << ' ';
    s << "OrderBook: "            << o.orderBook            << ' ';
    s << "Quantity: "             << o.quantity             << ' ';
    s << "User: "                 << o.user                 << ' ';
    s << "AppendageLength: "      << o.appendageLength;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
