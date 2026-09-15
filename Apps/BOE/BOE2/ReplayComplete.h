#ifndef ReplayComplete_h
#define ReplayComplete_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct ReplayComplete
{
    static constexpr MessageType Type = MessageType::ReplayComplete;

    MessageHeader header = { Type, sizeof(ReplayComplete) };
};
#pragma pack()

} // namespace BOE::BOE2

#endif
