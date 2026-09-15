#ifndef CancelOrder_h
#define CancelOrder_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct CancelOrder
{
    static constexpr MessageType Type = MessageType::CancelOrder;

    MessageHeader header = { Type, sizeof(CancelOrder) };

    Text<20>  origClOrdID;
    Binary<1> numberOfCancelOrderBitfields;
};
#pragma pack()

} // namespace BOE::BOE2

#endif
