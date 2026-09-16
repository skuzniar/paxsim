#ifndef CancelRejectedUSEquitiesV1_h
#define CancelRejectedUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct CancelRejectedUSEquitiesV1
{
    enum
    {
        Type = 0x119b
    };

    Header header = { Type, sizeof(CancelRejectedUSEquitiesV1) };

    Binary<2>  inFlight;
    DateTime   transactionTime;
    Alpha<4>   clearingFirm;
    Text<20>   clOrdID;
    ReasonCode cancelRejectReason;
    Text<60>   text;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
