#ifndef ModifyRejectedUSEquitiesV1_h
#define ModifyRejectedUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct ModifyRejectedUSEquitiesV1
{
    enum
    {
        Type = 0x1198
    };

    Header header = { Type, sizeof(ModifyRejectedUSEquitiesV1) };

    Binary<2>  inFlight;
    DateTime   transactionTime;
    Alpha<4>   clearingFirm;
    Text<20>   clOrdID;
    Text<20>   origClOrdID;
    ReasonCode modifyRejectReason;
    Text<60>   text;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
