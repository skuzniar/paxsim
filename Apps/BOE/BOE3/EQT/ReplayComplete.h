#ifndef ReplayComplete_h
#define ReplayComplete_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct ReplayComplete
{
    enum
    {
        Type = 502
    };

    Header header = { Type, sizeof(ReplayComplete) };
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
