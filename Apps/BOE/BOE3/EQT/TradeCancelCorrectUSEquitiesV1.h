#ifndef TradeCancelCorrectUSEquitiesV1_h
#define TradeCancelCorrectUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct TradeCancelCorrectUSEquitiesV1
{
    enum
    {
        Type = 0x119d
    };

    Header header = { Type, sizeof(TradeCancelCorrectUSEquitiesV1) };

    Binary<2>              inFlight;
    DateTime               transactionTime;
    Text<20>               clOrdID;
    Binary<8>              orderID;
    Binary<8>              execRefID;
    Side                   side;
    BaseLiquidityIndicator baseLiquidityIndicator;
    SubLiquidityIndicator  subLiquidityIndicator;
    Alpha<4>               clearingFirm;
    Text<4>                clearingAccount;
    Binary<4>              lastShares;
    BinaryPrice            lastPx;
    BinaryPrice            correctedPrice;
    DateTime               origTime;
    Alphanumeric<8>        symbol;
    Capacity               capacity;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
