#ifndef SystemEvent_h
#define SystemEvent_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct SystemEvent
{
    static constexpr char Type = 'S';

    SequencedData header = { Type, sizeof(SystemEvent) };
    Timestamp     timestamp;
    EventCode     eventCode;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
