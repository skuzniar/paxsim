#ifndef TradeBroken_h
#define TradeBroken_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct TradeBroken
{
    static constexpr char Type = 'B';

    enum class BreakReason : uint8_t
    {
        Erroneous   = 'E',
        Consent     = 'C',
        Supervisory = 'S',
        External    = 'X',
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, BreakReason o)
    {
        switch (o) {
                // clang-format off
            case BreakReason::Erroneous:   s << "'E'(Erroneous)";   break;
            case BreakReason::Consent:     s << "'C'(Consent)";     break;
            case BreakReason::Supervisory: s << "'S'(Supervisory)"; break;
            case BreakReason::External:    s << "'X'(External)";    break;
            default: s << std::to_string(static_cast<std::underlying_type_t<BreakReason>>(o)) + "(Invalid BreakReason)"; break;
                // clang-format on
        };
        return s;
    }

    template<typename T>
    static [[cppgen::auto]] T enum_cast(std::string_view v);

    template<>
    [[cppgen::auto]] BreakReason enum_cast<BreakReason>(std::string_view v)
    {
        // clang-format off
        if (v == "Erroneous")   return BreakReason::Erroneous;
        if (v == "Consent")     return BreakReason::Consent;
        if (v == "Supervisory") return BreakReason::Supervisory;
        if (v == "External")    return BreakReason::External;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of BreakReason enumeration range.");
    }

    SequencedData       header = { Type, sizeof(TradeBroken) };
    Timestamp           timestamp;
    UserRefNum          userRefNum;
    UInt32              matchNumber;
    BreakReason         reason;
    TradingMode         tradingMode;
    TransactionCategory transactionCategory;
    AlgoIndicator       algoIndicator;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const TradeBroken& o)
{
    // clang-format off
    s << "[TradeBroken]=";
    s << "Header: "              << o.header              << ' ';
    s << "Timestamp: "           << o.timestamp           << ' ';
    s << "UserRefNum: "          << o.userRefNum          << ' ';
    s << "MatchNumber: "         << o.matchNumber         << ' ';
    s << "Reason: "              << o.reason              << ' ';
    s << "TradingMode: "         << o.tradingMode         << ' ';
    s << "TransactionCategory: " << o.transactionCategory << ' ';
    s << "AlgoIndicator: "       << o.algoIndicator;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
