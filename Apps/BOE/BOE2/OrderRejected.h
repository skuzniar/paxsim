#ifndef OrderRejected_h
#define OrderRejected_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct OrderRejected
{
    static constexpr MessageType Type = MessageType::OrderRejected;

    MessageHeader header = { Type, sizeof(OrderRejected) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Text<1>   orderRejectReason;
    Text<60>  text;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace boe::boe2

#endif
