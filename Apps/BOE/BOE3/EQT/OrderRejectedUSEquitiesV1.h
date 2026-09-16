#ifndef OrderRejectedUSEquitiesV1_h
#define OrderRejectedUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct OrderRejectedUSEquitiesV1
{
    enum
    {
        Type = 0x1196
    };

    Header header = { Type, sizeof(OrderRejectedUSEquitiesV1) };

    Binary<2>  inFlight;
    DateTime   transactionTime;
    Text<20>   clOrdID;
    Alpha<4>   clearingFirm;
    ReasonCode orderRejectReason;
    Text<60>   text;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
