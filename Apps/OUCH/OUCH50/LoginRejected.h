#ifndef LoginRejected_h
#define LoginRejected_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct LoginRejected
{
    static constexpr char Type = 'J';

    enum class RejectCode : char
    {
        NotAuthorized       = 'A',
        SessionNotAvailable = 'S',
    };

    PacketHeader header = { Type, sizeof(LoginRejected) };
    RejectCode   rejectReasonCode;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
