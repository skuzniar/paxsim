#ifndef AccountQueryResponse_h
#define AccountQueryResponse_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct AccountQueryResponse
{
    static constexpr char Type = 'Q';

    SequencedData header = { Type, sizeof(AccountQueryResponse) };
    Timestamp     timestamp;
    UserRefNum    nextUserRefNum;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
