#ifndef CancelRejected_h
#define CancelRejected_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct CancelRejected
{
    static constexpr MessageType Type = MessageType::CancelRejected;

    MessageHeader header = { Type, sizeof(CancelRejected) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Text<1>   cancelRejectReason;
    Text<60>  text;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace boe::boe2

#endif
