#ifndef OrderCancelled_h
#define OrderCancelled_h

#include "PacketHeader.h"

namespace OUCH::OUCH50 {

#pragma pack(1)
struct OrderCancelled
{
    static constexpr char Type = 'C';

    enum class CancelReason : uint8_t
    {
        UserRequested       = 'U',
        ImmediateOrCancel   = 'I',
        Timeout             = 'T',
        Supervisory         = 'S',
        SelfMatchPrevention = 'Q',
        HiddenPegNoLIS      = 'L',
        BadQuote            = 'N',
        State               = 'R',
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, CancelReason o)
    {
        switch (o) {
                // clang-format off
            case CancelReason::UserRequested:       s << "'U'(UserRequested)";       break;
            case CancelReason::ImmediateOrCancel:   s << "'I'(ImmediateOrCancel)";   break;
            case CancelReason::Timeout:             s << "'T'(Timeout)";             break;
            case CancelReason::Supervisory:         s << "'S'(Supervisory)";         break;
            case CancelReason::SelfMatchPrevention: s << "'Q'(SelfMatchPrevention)"; break;
            case CancelReason::HiddenPegNoLIS:      s << "'L'(HiddenPegNoLIS)";      break;
            case CancelReason::BadQuote:            s << "'N'(BadQuote)";            break;
            case CancelReason::State:               s << "'R'(State)";               break;
            default: s << std::to_string(static_cast<std::underlying_type_t<CancelReason>>(o)) + "(Invalid CancelReason)"; break;
                // clang-format on
        };
        return s;
    }

    template<typename T>
    static [[cppgen::auto]] T enum_cast(std::string_view v);

    template<>
    [[cppgen::auto]] CancelReason enum_cast<CancelReason>(std::string_view v)
    {
        // clang-format off
        if (v == "UserRequested")       return CancelReason::UserRequested;
        if (v == "ImmediateOrCancel")   return CancelReason::ImmediateOrCancel;
        if (v == "Timeout")             return CancelReason::Timeout;
        if (v == "Supervisory")         return CancelReason::Supervisory;
        if (v == "SelfMatchPrevention") return CancelReason::SelfMatchPrevention;
        if (v == "HiddenPegNoLIS")      return CancelReason::HiddenPegNoLIS;
        if (v == "BadQuote")            return CancelReason::BadQuote;
        if (v == "State")               return CancelReason::State;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of CancelReason enumeration range.");
    }

    SequencedData header = { Type, sizeof(OrderCancelled) };
    Timestamp     timestamp;
    UserRefNum    userRefNum;
    Int32         decrementQuantity;
    CancelReason  reason;
    // Appendages;
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const OrderCancelled& o)
{
    // clang-format off
    s << "[OrderCancelled]=";
    s << "Header: "            << o.header            << ' ';
    s << "Timestamp: "         << o.timestamp         << ' ';
    s << "UserRefNum: "        << o.userRefNum        << ' ';
    s << "DecrementQuantity: " << o.decrementQuantity << ' ';
    s << "Reason: "            << o.reason;
    // clang-format on
    return s;
}

} // namespace OUCH::OUCH50

#endif
