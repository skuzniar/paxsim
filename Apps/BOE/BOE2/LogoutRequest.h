#ifndef LogoutRequest_h
#define LogoutRequest_h

#include "MessageHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct LogoutRequest
{
    static constexpr MessageType Type = MessageType::LogoutRequest;

    MessageHeader header = { Type, sizeof(LogoutRequest) };
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

} // namespace BOE::BOE2

#endif
