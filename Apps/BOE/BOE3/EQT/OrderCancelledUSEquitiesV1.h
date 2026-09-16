#ifndef OrderCancelledUSEquitiesV1_h
#define OrderCancelledUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderCancelledUSEquitiesV1
{
    enum
    {
        Type = 0x119a
    };

    Header header = { Type, sizeof(OrderCancelledUSEquitiesV1) };

    Binary<2>  inFlight;
    DateTime   transactionTime;
    Alpha<4>   clearingFirm;
    Text<20>   clOrdID;
    ReasonCode cancelReason;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
