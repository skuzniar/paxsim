#ifndef LoginRequest_h
#define LoginRequest_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct LoginRequest
{
    static constexpr char Type = 'L';

    static constexpr std::size_t size()
    {
        return sizeof(LoginRequest);
    }

    LoginRequest() = default;

    LoginRequest(std::string_view user, std::string_view password)
      : userName(user)
      , password(password)
    {
    }

    PacketHeader header = { Type, sizeof(LoginRequest) };
    Alpha<6>     userName;
    Alpha<10>    password;
    Alpha<10>    requestedSession;
    Alpha<20>    requestedSequenceNumber = 0;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginRequest& o)
{
    // clang-format off
    s << "[LoginRequest]=";
    s << "Header: "                  << o.header                  << ' ';
    s << "UserName: "                << o.userName                << ' ';
    s << "Password: "                << o.password                << ' ';
    s << "RequestedSession: "        << o.requestedSession        << ' ';
    s << "RequestedSequenceNumber: " << o.requestedSequenceNumber;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
