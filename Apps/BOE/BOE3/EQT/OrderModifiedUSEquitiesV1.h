#ifndef OrderModifiedUSEquitiesV1_h
#define OrderModifiedUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderModifiedUSEquitiesV1
{
    enum
    {
        Type = 0x1197
    };

    Header header = { Type, sizeof(OrderModifiedUSEquitiesV1) };

    Binary<2>              inFlight;
    DateTime               transactionTime;
    Text<20>               clOrdID;
    Text<20>               origClOrdID;
    Binary<8>              orderID;
    Alpha<4>               clearingFirm;
    Alphanumeric<8>        symbol;
    Side                   side;
    BinaryPrice            price;
    OrdType                ordType;
    Binary<4>              maxFloor;
    Binary<4>              orderQty;
    BinaryPrice            stopPx;
    Binary<4>              leavesQty;
    BinaryPrice            displayPrice;
    BinaryPrice            workingPrice;
    BaseLiquidityIndicator baseLiquidityIndicator;
    Binary<4>              secondaryOrderID;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
