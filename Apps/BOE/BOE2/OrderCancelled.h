#ifndef OrderCancelled_h
#define OrderCancelled_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct OrderCancelled
{
    static constexpr MessageType Type = MessageType::OrderCancelled;

    MessageHeader header = { Type, sizeof(OrderCancelled) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Text<1>   cancelReason;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace boe::boe2

#endif
