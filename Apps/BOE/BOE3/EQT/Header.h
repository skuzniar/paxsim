#ifndef Header_h
#define Header_h

#include "Types.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct Header
{
    Binary<2> startOfMessage = 0xE3B0;
    Binary<2> messageLength;
    Binary<2> messageType;
    Binary<1> matchingUnit   = 0;
    Binary<1> reserved       = 0;
    Binary<4> sequenceNumber = 0;

    Header() = default;

    Header(uint16_t type, uint16_t length, uint32_t sequence = 0)
      : messageLength(length - sizeof(startOfMessage))
      , messageType(type)
      , sequenceNumber(sequence)
    {
    }

    const char* data() const
    {
        return reinterpret_cast<const char*>(this);
    }

    std::size_t length() const
    {
        return messageLength + sizeof(startOfMessage);
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const Header& o)
{
    // clang-format off
    s << "[Header]=";
    s << "StartOfMessage: " << o.startOfMessage << ' ';
    s << "MessageLength: "  << o.messageLength  << ' ';
    s << "MessageType: "    << o.messageType    << ' ';
    s << "MatchingUnit: "   << o.matchingUnit   << ' ';
    s << "Reserved: "       << o.reserved       << ' ';
    s << "SequenceNumber: " << o.sequenceNumber;
    // clang-format on
    return s;
}

} // namespace boe::boe3::eqt

#endif
