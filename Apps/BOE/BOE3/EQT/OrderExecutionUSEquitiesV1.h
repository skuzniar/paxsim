#ifndef OrderExecutionUSEquitiesV1_h
#define OrderExecutionUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderExecutionUSEquitiesV1
{
    enum
    {
        Type = 0x119c
    };

    Header header = { Type, sizeof(OrderExecutionUSEquitiesV1) };

    Binary<2>              inFlight;
    DateTime               transactionTime;
    Alpha<4>               clearingFirm;
    Text<20>               clOrdID;
    Binary<8>              execID;
    Binary<4>              lastShares;
    BinaryPrice            lastPx;
    Binary<4>              leavesQty;
    BaseLiquidityIndicator baseLiquidityIndicator;
    SubLiquidityIndicator  subLiquidityIndicator;
    Alphanumeric<4>        contraBroker;
    Side                   side;
    Alphanumeric<8>        symbol;
    FeeCode                feeCode;
    BinaryFee              accessFee;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
