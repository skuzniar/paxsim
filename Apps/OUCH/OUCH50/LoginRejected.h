#ifndef LoginRejected_h
#define LoginRejected_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct LoginRejected
{
    static constexpr char Type = 'J';

    enum class RejectCode : char
    {
        NotAuthorized       = 'A',
        SessionNotAvailable = 'S',
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, RejectCode o)
    {
        switch (o) {
                // clang-format off
            case RejectCode::NotAuthorized:       s << "'A'(NotAuthorized)";       break;
            case RejectCode::SessionNotAvailable: s << "'S'(SessionNotAvailable)"; break;
            default: s << std::to_string(static_cast<std::underlying_type_t<RejectCode>>(o)) + "(Invalid RejectCode)"; break;
                // clang-format on
        };
        return s;
    }

    PacketHeader header = { Type, sizeof(LoginRejected) };
    RejectCode   rejectReasonCode;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LoginRejected& o)
{
    // clang-format off
    s << "[LoginRejected]=";
    s << "Header: "           << o.header           << ' ';
    s << "RejectReasonCode: " << o.rejectReasonCode;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
