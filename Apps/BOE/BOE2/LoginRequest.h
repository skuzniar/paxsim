#ifndef LoginRequest_h
#define LoginRequest_h

#include "BOE/Traits.h"

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct LoginRequest
{
    static constexpr MessageType Type = MessageType::LoginRequest;

    LoginRequest() = default;

    LoginRequest(std::string_view user, std::string_view password)
      : username(user)
      , password(password)
    {
    }

    MessageHeader    header = { Type, sizeof(LoginRequest) };
    Alphanumeric<4>  sessionSubID;
    Alphanumeric<4>  username;
    Alphanumeric<10> password;
    Binary<1>        numberOfParams;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginRequest& o)
{
    // clang-format off
    s << "[LoginRequest]=";
    s << "Header: "         << o.header         << ' ';
    s << "SessionSubID: "   << o.sessionSubID   << ' ';
    s << "Username: "       << o.username       << ' ';
    s << "Password: "       << o.password       << ' ';
    s << "NumberOfParams: " << o.numberOfParams;
    // clang-format on
    return s;
}

struct UnitSequencesParameterGroup
{
    Binary<2> paramGroupLength;
    Binary<1> paramGroupType = 0x80;
    Binary<1> noUnspecifiedUnitReplay;
    Binary<1> numberOfUnits;
};

struct UnitSequence
{
    Binary<1> unitNumber;
    Binary<4> unitSequence;
};

struct ReturnBitfieldsParameterGroup
{
    Binary<2> paramGroupLength;
    Binary<1> paramGroupType = 0x81;
    Binary<1> messageType;
    Binary<1> numberOfReturnBitfields;
};

struct ReturnBitfield
{
    Binary<1> returnBitfield;
};

} // namespace boe::boe2

namespace boe {
//-----------------------------------------------------------------------------------------------------------------
// Message traits specialization.
//-----------------------------------------------------------------------------------------------------------------
template<>
class Authentication<boe2::LoginRequest>
{
public:
    static std::string_view user(const boe2::LoginRequest& msg)
    {
        return msg.username;
    }
    static std::string_view password(const boe2::LoginRequest& msg)
    {
        return msg.password;
    }
};

} // namespace boe

#endif
