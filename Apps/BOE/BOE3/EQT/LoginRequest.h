#ifndef LoginRequest_h
#define LoginRequest_h

#include "BOE/Traits.h"

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct LoginRequest
{
    static constexpr uint32_t MAX_MATCH_UNITS = std::numeric_limits<uint8_t>::max();

    enum
    {
        Type = 1
    };

    Header             header = { Type, sizeof(LoginRequest) };
    Alphanumeric<4>    sessionId;
    Alphanumeric<4>    sessionSubId;
    Alphanumeric<10>   password;
    ReplayInstructions replayUnspecifiedUnit = ReplayInstructions::Replay;
    Binary<1>          numberOfUnits         = 0;

    struct Unit
    {
        Binary<1> unitNumber;
        Binary<4> unitSequence;
    };

    LoginRequest() = default;

    LoginRequest(std::string_view user, std::string_view password)
      : sessionId(user)
      , password(password)
    {
    }

    static std::size_t max_size()
    {
        return sizeof(LoginRequest) + MAX_MATCH_UNITS * sizeof(Unit);
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginRequest& o)
{
    // clang-format off
    s << "[LoginRequest]=";
    s << "Header: "                << o.header                << ' ';
    s << "SessionId: "             << o.sessionId             << ' ';
    s << "SessionSubId: "          << o.sessionSubId          << ' ';
    s << "Password: "              << o.password              << ' ';
    s << "ReplayUnspecifiedUnit: " << o.replayUnspecifiedUnit << ' ';
    s << "NumberOfUnits: "         << o.numberOfUnits;
    // clang-format on
    return s;
}

inline LoginRequest&
operator+=(LoginRequest& o, const LoginRequest::Unit& u)
{
    auto* offset = reinterpret_cast<char*>(std::addressof(o)) + o.header.length();
    new (offset) LoginRequest::Unit(u);
    o.numberOfUnits        = o.numberOfUnits + 1;
    o.header.messageLength = o.header.messageLength + sizeof(LoginRequest::Unit);
    return o;
}

} // namespace boe::boe3::eqt

namespace boe {
//-----------------------------------------------------------------------------------------------------------------
// Message traits specialization.
//-----------------------------------------------------------------------------------------------------------------
template<>
class Authentication<boe3::eqt::LoginRequest>
{
public:
    static std::string_view user(const boe3::eqt::LoginRequest& msg)
    {
        return msg.sessionId;
    }
    static std::string_view password(const boe3::eqt::LoginRequest& msg)
    {
        return msg.password;
    }
};

} // namespace boe

#endif
