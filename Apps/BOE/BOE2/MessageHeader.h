#ifndef BOE_BOE2_MessageHeader_h
#define BOE_BOE2_MessageHeader_h

#include "Types.h"

namespace boe::boe2 {

#pragma pack(1)
struct MessageHeader
{
    MessageHeader(MessageType type, uint16_t size)
      : messageLength(size - sizeof(startOfMessage))
      , messageType(type)
    {
    }

    std::size_t size() const
    {
        return messageLength + sizeof(startOfMessage);
    }

    const char* data() const
    {
        return reinterpret_cast<const char*>(this);
    }
    Binary<2>   startOfMessage = 0xBABA;
    Binary<2>   messageLength;
    MessageType messageType;
    Binary<1>   matchingUnit;
    Binary<4>   sequenceNumber;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const MessageHeader& o)
{
    // clang-format off
    s << "[MessageHeader]=";
    s << "StartOfMessage: " << o.startOfMessage << ' ';
    s << "MessageLength: "  << o.messageLength  << ' ';
    s << "MessageType: "    << o.messageType    << ' ';
    s << "MatchingUnit: "   << o.matchingUnit   << ' ';
    s << "SequenceNumber: " << o.sequenceNumber;
    // clang-format on
    return s;
}

} // namespace boe::boe2

#endif
