#ifndef EndOfSession_h
#define EndOfSession_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct EndOfSession
{
    static constexpr char Type = 'Z';

    PacketHeader header = { Type, sizeof(EndOfSession) };
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
