#ifndef ClientHeartbeat_h
#define ClientHeartbeat_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct ClientHeartbeat
{
    static constexpr char Type = 'R';

    static constexpr std::size_t size()
    {
        return sizeof(ClientHeartbeat);
    }

    PacketHeader header = { Type, sizeof(ClientHeartbeat) };
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
