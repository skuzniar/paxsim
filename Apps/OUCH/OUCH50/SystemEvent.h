#ifndef SystemEvent_h
#define SystemEvent_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct SystemEvent
{
    static constexpr char Type = 'S';

    SequencedData header = { Type, sizeof(SystemEvent) };
    Timestamp     timestamp;
    EventCode     eventCode;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const SystemEvent& o)
{
    // clang-format off
    s << "[SystemEvent]=";
    s << "Header: "    << o.header    << ' ';
    s << "Timestamp: " << o.timestamp << ' ';
    s << "EventCode: " << o.eventCode;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
