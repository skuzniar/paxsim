#ifndef LogoutResponse_h
#define LogoutResponse_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct LogoutResponse
{
    enum
    {
        Type = 503
    };

    Header       header = { Type, sizeof(LogoutResponse) };
    LogoutReason logoutReason;
    Text<60>     logoutReasonText;

    LogoutResponse() = default;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
