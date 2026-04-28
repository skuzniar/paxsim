#ifndef OrderRestated_h
#define OrderRestated_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderRestated
{
    static constexpr char Type = 'T';

    enum class Reason : uint8_t
    {
        RefreshOfDisplay       = 'R',
        UpdateOfDisplayedPrice = 'P',
    };

    SequencedData header = { Type, sizeof(OrderRestated) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    Reason        reason;
    Int16         appendageLength;

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
