#ifndef OrderAcknowledgementUSEquitiesV1_h
#define OrderAcknowledgementUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderAcknowledgementUSEquitiesV1
{
    enum
    {
        Type = 0x1195
    };

    Header header = { Type, sizeof(OrderAcknowledgementUSEquitiesV1) };

    Binary<2>              inFlight;
    DateTime               transactionTime;
    Text<20>               clOrdID;
    Binary<8>              orderID;
    Side                   side;
    BinaryPrice            price;
    OrdType                ordType;
    Alphanumeric<8>        symbol;
    Alpha<4>               clearingFirm;
    Binary<4>              leavesQty;
    BinaryPrice            displayPrice;
    BinaryPrice            workingPrice;
    BaseLiquidityIndicator baseLiquidityIndicator;
    SubLiquidityIndicator  subLiquidityIndicator;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
