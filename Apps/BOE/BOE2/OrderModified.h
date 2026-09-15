#ifndef OrderModified_h
#define OrderModified_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct OrderModified
{
    static constexpr MessageType Type = MessageType::OrderModified;

    MessageHeader header = { Type, sizeof(OrderModified) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Binary<8> orderID;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace BOE::BOE2

#endif
