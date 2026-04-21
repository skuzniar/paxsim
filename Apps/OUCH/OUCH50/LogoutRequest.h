#ifndef LogoutRequest_h
#define LogoutRequest_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct LogoutRequest
{
    static constexpr char Type = 'O';

    PacketHeader header = { Type, sizeof(LogoutRequest) };

    std::size_t size() const
    {
        return header.size();
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LogoutRequest& o)
{
    // clang-format off
    s << "[LogoutRequest]=";
    s << "Header: " << o.header;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
