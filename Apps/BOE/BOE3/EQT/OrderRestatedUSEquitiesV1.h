#ifndef OrderRestatedUSEquitiesV1_h
#define OrderRestatedUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderRestatedUSEquitiesV1
{
    enum
    {
        Type = 0x1199
    };

    Header header = { Type, sizeof(OrderRestatedUSEquitiesV1) };

    Binary<2>              inFlight;
    DateTime               transactionTime;
    Text<20>               clOrdID;
    Binary<8>              orderID;
    Alpha<4>               clearingFirm;
    Alphanumeric<8>        symbol;
    RestatementReason      orderRestatementReason;
    Binary<4>              orderQty;
    BinaryPrice            price;
    Binary<4>              leavesQty;
    BinaryPrice            displayPrice;
    BinaryPrice            workingPrice;
    BaseLiquidityIndicator baseLiquidityIndicator;
    Binary<8>              secondaryOrderID;
    Binary<4>              lastShares;
    BinaryPrice            lastPx;
    Binary<4>              cmcMatchQty;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
