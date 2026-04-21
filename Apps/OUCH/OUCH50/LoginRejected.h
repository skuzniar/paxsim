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

    friend std::string to_json(RejectCode o, bool verbose)
    {
        if (verbose) {
            switch (o) {
                    // clang-format off
            	case RejectCode::NotAuthorized:       return to_json("'A'(NotAuthorized)"      , verbose); break;
            	case RejectCode::SessionNotAvailable: return to_json("'S'(SessionNotAvailable)", verbose); break;
            	default: return to_json(std::to_string(static_cast<std::underlying_type_t<RejectCode>>(o)) + "(Invalid LoginReject)", verbose); break;
                    // clang-format on
            };
        } else {
            switch (o) {
                    // clang-format off
            	case RejectCode::NotAuthorized:       return to_json('A', verbose); break;
            	case RejectCode::SessionNotAvailable: return to_json('S', verbose); break;
            	default: return to_json(static_cast<std::underlying_type_t<RejectCode>>(o), verbose); break;
                    // clang-format on
            };
        }
        return to_json("", verbose);
    }

    PacketHeader header = { Type, sizeof(LoginRejected) };
    RejectCode   rejectReasonCode;
};
#pragma pack()

} // namespace OUCH::OUCH50

#endif
