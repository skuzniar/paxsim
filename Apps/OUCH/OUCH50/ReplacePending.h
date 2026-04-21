#ifndef ReplacePending_h
#define ReplacePending_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ReplacePending
{
    static constexpr char Type = 'N';

    SequencedData header = { Type, sizeof(ReplacePending) };
    Timestamp     timestamp;
    UserRefNum    origUserRefNum;
    UserRefNum    newUserRefNum;
    PendingReason reason;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
