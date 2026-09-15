#ifndef LogoutResponse_h
#define LogoutResponse_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct LogoutResponse
{
    static constexpr MessageType Type = MessageType::LoginResponse;

    MessageHeader header = { Type, sizeof(LogoutResponse) };

    Alphanumeric<1> logoutReason;
    Text<60>        logoutReasonText;
    Binary<4>       lastReceivedSequenceNumber;
    Binary<1>       numberOfUnits;
};
#pragma pack()

} // namespace BOE::BOE2

#endif
