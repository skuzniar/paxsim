#ifndef OrderAcknowledgement_h
#define OrderAcknowledgement_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct OrderAcknowledgement
{
    static constexpr MessageType Type = MessageType::OrderAcknowledgment;

    MessageHeader header = { Type, sizeof(OrderAcknowledgement) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Binary<8> orderID;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace BOE::BOE2

#endif
