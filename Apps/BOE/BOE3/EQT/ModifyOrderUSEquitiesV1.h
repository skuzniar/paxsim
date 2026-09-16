#ifndef ModifyOrderUSEquitiesV1_h
#define ModifyOrderUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct ModifyOrderUSEquitiesV1
{
    enum
    {
        Type = 0x0fa2
    };

    Header header = { Type, sizeof(ModifyOrderUSEquitiesV1) };

    Text<20>        clOrdID;
    Text<20>        origClOrdID;
    Alpha<4>        clearingFirm;
    Alphanumeric<8> symbol;
    Binary<4>       orderQty;
    BinaryPrice     price;
    OrdType         ordType = OrdType::Limit;
    Side            side    = Side::Buy;
    Binary<4>       maxFloor;
    BinaryPrice     stopPx;
    YesNo           cancelOrigOnReject = YesNo::No;
    Text<4>         locateBroker;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
