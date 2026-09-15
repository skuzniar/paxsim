#ifndef ReplayComplete_h
#define ReplayComplete_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct ReplayComplete
{
    static constexpr MessageType Type = MessageType::ReplayComplete;

    MessageHeader header = { Type, sizeof(ReplayComplete) };
};
#pragma pack()

} // namespace boe::boe2

#endif
