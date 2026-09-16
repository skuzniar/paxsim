#ifndef CancelOrderUSEquitiesV1_h
#define CancelOrderUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct CancelOrderUSEquitiesV1
{
    enum
    {
        Type = 0xfa3
    };

    Header header = { Type, sizeof(CancelOrderUSEquitiesV1) };

    Text<20> origClOrdID;
    Alpha<4> clearingFirm;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
