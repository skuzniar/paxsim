#ifndef RefreshRequest_h
#define RefreshRequest_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct RefreshRequest
{
    static constexpr char Type = 'W';

    enum class Reason : uint8_t
    {
        PassiveMMO    = 'P',
        AggressiveMMO = 'A',
    };

    SequencedData header = { Type, sizeof(RefreshRequest) };
    Timestamp     timestamp;
    Alpha<4>      firm;
    Int32         orderBook;
    Reason        reason;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
