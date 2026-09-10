#ifndef OrderRestated_h
#define OrderRestated_h

#include "PacketHeader.h"
#include "Appendage.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct OrderRestated
{
    static constexpr char Type = 'T';

    enum class Reason : uint8_t
    {
        RefreshOfDisplay       = 'R',
        UpdateOfDisplayedPrice = 'P',
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, Reason o)
    {
        switch (o) {
                // clang-format off
            case Reason::RefreshOfDisplay:       s << "'R'(RefreshOfDisplay)";       break;
            case Reason::UpdateOfDisplayedPrice: s << "'P'(UpdateOfDisplayedPrice)"; break;
            default: s << std::to_string(static_cast<std::underlying_type_t<Reason>>(o)) + "(Invalid Reason)"; break;
                // clang-format on
        };
        return s;
    }

    SequencedData header = { Type, sizeof(OrderRestated) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    Reason        reason;
    UInt16        appendageLength;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const OrderRestated& o)
{
    // clang-format off
    s << "[OrderRestated]=";
    s << "Header: "          << o.header          << ' ';
    s << "Timestamp: "       << o.timestamp       << ' ';
    s << "UserRefNum: "      << o.userRefNum      << ' ';
    s << "Reason: "          << o.reason          << ' ';
    s << "AppendageLength: " << o.appendageLength;
    // clang-format on
    return s;
}

} // namespace BOE::BOE2

#endif
