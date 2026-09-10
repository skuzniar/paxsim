#ifndef RefreshRequest_h
#define RefreshRequest_h

#include "PacketHeader.h"

namespace BOE::BOE2 {

#pragma pack(1)
struct RefreshRequest
{
    static constexpr char Type = 'W';

    enum class Reason : uint8_t
    {
        PassiveMMO    = 'P',
        AggressiveMMO = 'A',
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, Reason o)
    {
        switch (o) {
                // clang-format off
            case Reason::PassiveMMO:    s << "'P'(PassiveMMO)";    break;
            case Reason::AggressiveMMO: s << "'A'(AggressiveMMO)"; break;
            default: s << std::to_string(static_cast<std::underlying_type_t<Reason>>(o)) + "(Invalid Reason)"; break;
                // clang-format on
        };
        return s;
    }

    SequencedData header = { Type, sizeof(RefreshRequest) };
    Timestamp     timestamp;
    Alpha<4>      firm;
    UInt32        orderBook;
    Reason        reason;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const RefreshRequest& o)
{
    // clang-format off
    s << "[RefreshRequest]=";
    s << "Header: "    << o.header    << ' ';
    s << "Timestamp: " << o.timestamp << ' ';
    s << "Firm: "      << o.firm      << ' ';
    s << "OrderBook: " << o.orderBook << ' ';
    s << "Reason: "    << o.reason;
    // clang-format on
    return s;
}

} // namespace BOE::BOE2

#endif
