#ifndef CancelPending_h
#define CancelPending_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct CancelPending
{
    static constexpr char Type = 'P';

    SequencedData header = { Type, sizeof(CancelPending) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    PendingReason reason;
    // Appendages;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
