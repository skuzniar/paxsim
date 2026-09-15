#ifndef UserModifyRejected_h
#define UserModifyRejected_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct UserModifyRejected
{
    static constexpr MessageType Type = MessageType::UserModifyRejected;

    MessageHeader header = { Type, sizeof(UserModifyRejected) };

    DateTime  transactionTime;
    Text<20>  clOrdID;
    Text<1>   modifyRejectReason;
    Text<60>  text;
    Binary<1> reservedInternal;
    Binary<1> numberOfReturnBitfields;
};
#pragma pack()

} // namespace boe::boe2

#endif
