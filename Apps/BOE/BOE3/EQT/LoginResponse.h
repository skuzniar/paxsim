#ifndef LoginResponse_h
#define LoginResponse_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct LoginResponse
{
    enum
    {
        Type = 501
    };

    Header              header = { Type, sizeof(LoginResponse) };
    LoginResponseStatus loginResponseStatus;
    Text<60>            loginResponseText;
    Binary<4>           clientSequence;
    Binary<1>           numberOfUnits;

    struct Unit
    {
        Binary<1> unitNumber;
        Binary<4> unitSequence;
    };

    LoginResponse() = default;

    LoginResponse(LoginResponseStatus status)
      : loginResponseStatus(status)
    {
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginResponse& o)
{
    // clang-format off
    s << "[LoginResponse]=";
    s << "Header: "              << o.header              << ' ';
    s << "LoginResponseStatus: " << o.loginResponseStatus << ' ';
    s << "LoginResponseText: "   << o.loginResponseText   << ' ';
    s << "ClientSequence: "      << o.clientSequence      << ' ';
    s << "NumberOfUnits: "       << o.numberOfUnits;
    // clang-format on
    return s;
}

} // namespace boe::boe3::eqt

#endif
