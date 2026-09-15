#ifndef LoginResponse_h
#define LoginResponse_h

#include "MessageHeader.h"

namespace boe::boe2 {

enum class LoginResponseStatus : char
{
    LoginAccepted                       = 'A',
    NotAuthorized                       = 'N',
    SessionIsDisabled                   = 'D',
    SessionInUse                        = 'B',
    InvalidSession                      = 'S',
    SequenceAheadInLoginMessage         = 'Q',
    InvalidUnitGivenInLoginMessage      = 'I',
    InvalidReturnBitFieldInLoginMessage = 'F',
    InvalidLoginRequestMessageStructure = 'M',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LoginResponseStatus o)
{
    switch (o) {
            // clang-format off
        case LoginResponseStatus::LoginAccepted:                       s << "'A'(LoginAccepted)";                       break;
        case LoginResponseStatus::NotAuthorized:                       s << "'N'(NotAuthorized)";                       break;
        case LoginResponseStatus::SessionIsDisabled:                   s << "'D'(SessionIsDisabled)";                   break;
        case LoginResponseStatus::SessionInUse:                        s << "'B'(SessionInUse)";                        break;
        case LoginResponseStatus::InvalidSession:                      s << "'S'(InvalidSession)";                      break;
        case LoginResponseStatus::SequenceAheadInLoginMessage:         s << "'Q'(SequenceAheadInLoginMessage)";         break;
        case LoginResponseStatus::InvalidUnitGivenInLoginMessage:      s << "'I'(InvalidUnitGivenInLoginMessage)";      break;
        case LoginResponseStatus::InvalidReturnBitFieldInLoginMessage: s << "'F'(InvalidReturnBitFieldInLoginMessage)"; break;
        case LoginResponseStatus::InvalidLoginRequestMessageStructure: s << "'M'(InvalidLoginRequestMessageStructure)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LoginResponseStatus>>(o)) + "(Invalid LoginResponseStatus)"; break;
            // clang-format on
    };
    return s;
}

#pragma pack(1)
struct LoginResponse
{
    static constexpr MessageType Type = MessageType::LoginResponse;

    MessageHeader header = { Type, sizeof(LoginResponse) };

    LoginResponse() = default;

    LoginResponse(LoginResponseStatus staus)
      : loginResponseStatus(staus)
    {
    }

    LoginResponse(LoginResponseStatus staus, std::string_view text)
      : loginResponseStatus(staus)
      , loginResponseText(text)
    {
    }

    LoginResponseStatus loginResponseStatus;
    Text<60>            loginResponseText;
    Binary<1>           noUnspecifiedUnitReplay;
    Binary<4>           lastReceivedSequenceNumber;
    Binary<1>           numberOfUnits;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginResponse& o)
{
    // clang-format off
    s << "[LoginResponse]=";
    s << "Header: "                     << o.header                     << ' ';
    s << "LoginResponseStatus: "        << o.loginResponseStatus        << ' ';
    s << "LoginResponseText: "          << o.loginResponseText          << ' ';
    s << "NoUnspecifiedUnitReplay: "    << o.noUnspecifiedUnitReplay    << ' ';
    s << "LastReceivedSequenceNumber: " << o.lastReceivedSequenceNumber << ' ';
    s << "NumberOfUnits: "              << o.numberOfUnits;
    // clang-format on
    return s;
}

} // namespace boe::boe2

#endif
