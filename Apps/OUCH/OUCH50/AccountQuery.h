#ifndef AccountQuery_h
#define AccountQuery_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct AccountQuery
{
    static constexpr char Type = 'Q';

    UnsequencedData header = { Type, sizeof(AccountQuery) };

    static constexpr std::size_t size()
    {
        return sizeof(AccountQuery);
    }
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
