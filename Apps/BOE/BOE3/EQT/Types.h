#ifndef Types_h
#define Types_h

#include <algorithm>
#include <cmath>
#include <charconv>
#include <chrono>
#include <map>

namespace boe::boe3::eqt {

//---------------------------------------------------------------------------------------------------------------------
// Binary. Little Endian byte order, unsigned binary value. The number of bytes used depends on the context.
//---------------------------------------------------------------------------------------------------------------------
template<size_t N>
struct Binary;

#pragma pack(1)
template<>
struct Binary<1>
{
    uint8_t value = 0;

    Binary() = default;

    Binary(uint8_t v)
      : value(v)
    {
    }

    operator uint8_t() const
    {
        return value;
    }

    uint8_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Binary& o)
    {
        return os << unsigned(o.value);
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct Binary<2>
{
    uint16_t value = 0;

    Binary() = default;

    Binary(uint16_t v)
      : value(v)
    {
    }

    operator uint16_t() const
    {
        return value;
    }

    uint16_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Binary& o)
    {
        return os << o.value;
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct Binary<4>
{
    uint32_t value = 0;

    Binary() = default;

    Binary(uint32_t v)
      : value(v)
    {
    }

    operator uint32_t() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Binary& o)
    {
        return os << o.value;
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct Binary<8>
{
    uint64_t value = 0;

    Binary() = default;

    Binary(uint64_t v)
      : value(v)
    {
    }

    operator uint64_t() const
    {
        return value;
    }

    uint64_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Binary& o)
    {
        return os << o.value;
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Price: Little Endian byte order value, signed two's complement, eight bytes, DP implied decimal places.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<std::size_t DP>
struct Price
{
    // DP implied decimal places
    static constexpr long multiplier = std::pow(10, DP);

    int64_t value = 0;

    Price() = default;

    Price(double v)
      : value(std::lrint(v * multiplier))
    {
    }

    Price& operator=(double v)
    {
        value = std::lrint(v * multiplier);
        return *this;
    }

    operator double() const
    {
        auto div = lldiv(value, multiplier);
        return div.quot + double(div.rem) / multiplier;
    }

    friend std::ostream& operator<<(std::ostream& os, const Price& p)
    {
        os << p.value;
        return os;
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Short Price: Little Endian byte order value, signed two's complement, four bytes, DP implied decimal places.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<std::size_t DP>
struct ShortPrice
{
    // DP implied decimal places
    static constexpr long multiplier = std::pow(10, DP);

    int32_t value = 0;

    ShortPrice() = default;

    ShortPrice(double v)
      : value(std::lrint(v * multiplier))
    {
    }

    ShortPrice& operator=(double v)
    {
        value = std::lrint(v * multiplier);
        return *this;
    }

    operator double() const
    {
        auto div = lldiv(value, multiplier);
        return div.quot + double(div.rem) / multiplier;
    }

    friend std::ostream& operator<<(std::ostream& os, const ShortPrice& p)
    {
        os << p.value;
        return os;
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Binary Price , Short Binary Price and Binary Fee.
//---------------------------------------------------------------------------------------------------------------------
using BinaryPrice      = Price<4>;
using ShortBinaryPrice = ShortPrice<4>;
using BinaryFee        = Price<5>;

//---------------------------------------------------------------------------------------------------------------------
// Alpha. ASCII (A-Z), (a-z) only. ASCII NUL (0x00) filled on the right, if necessary.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<size_t N>
struct Alpha
{
    std::array<char, N> value{};

    Alpha() = default;

    Alpha(std::string_view v)
    {
        std::size_t len = std::min(value.size(), v.size());
        std::copy(v.data(), v.data() + len, value.data());
        std::fill(value.data() + len, value.end(), '\0');
    }

    Alpha(const char* v)
      : Alpha(std::string_view(v))
    {
    }

    Alpha(char v)
      : Alpha(std::string_view(&v, 1))
    {
    }

    template<size_t K>
    Alpha(const char (&array)[K])
      : Alpha(std::string_view(std::addressof(array[0]), K))
    {
    }

    Alpha& operator=(std::string_view v)
    {
        std::size_t len = std::min(value.size(), v.size());
        std::copy(v.data(), v.data() + len, value.data());
        std::fill(value.data() + len, value.end(), '\0');
        return *this;
    }

    Alpha& operator=(const char* v)
    {
        *this = std::string_view(v);
        return *this;
    }

    Alpha& operator=(const char v)
    {
        *this = Alpha(v);
        return *this;
    }

    template<size_t K>
    Alpha& operator=(const char (&array)[K])
    {
        *this = std::string_view(std::addressof(array[0]), K);
        return *this;
    }

    char& operator[](size_t index)
    {
        return value[index];
    }

    const char& operator[](size_t index) const
    {
        return value[index];
    }

    operator std::string_view() const
    {
        return { value.data(), static_cast<std::size_t>(std::distance(value.begin(), std::find(value.begin(), value.end(), 0))) };
    }

    std::string to_string() const
    {
        return std::string(value.begin(), static_cast<size_t>(std::distance(value.begin(), std::find(value.begin(), value.end(), 0))));
    }

    [[nodiscard]] char* begin()
    {
        return value.begin();
    }
    [[nodiscard]] char* end()
    {
        return value.end();
    }

    [[nodiscard]] const char* begin() const
    {
        return value.begin();
    }
    [[nodiscard]] const char* end() const
    {
        return value.end();
    }

    auto size() const
    {
        return value.size();
    }

    bool empty() const
    {
        return value[0] == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Alpha& s)
    {
        os.write(s.value.data(), std::distance(s.value.begin(), std::find(s.value.begin(), s.value.end(), 0)));
        return os;
    }

    template<typename T>
    bool from_chars(T& v)
    {
        auto r = std::from_chars(value.begin(), value.end(), v);
        // When converting into a number we want to ensure that all content is consumed
        return r.ec == std::errc() && (r.ptr == value.end() || *r.ptr == 0);
    };
};
#pragma pack()

template<size_t N>
std::string
to_string(const Alpha<N>& a)
{
    return { a.value.data(), static_cast<std::size_t>(std::distance(a.value.begin(), std::find(a.value.begin(), a.value.end(), 0))) };
}

//---------------------------------------------------------------------------------------------------------------------
// Alphanumeric. ASCII (A-Z), (a-z), (0-9) only. ASCII NUL (0x00) filled on the right, if necessary.
//---------------------------------------------------------------------------------------------------------------------
template<size_t N>
using Alphanumeric = Alpha<N>;

//---------------------------------------------------------------------------------------------------------------------
// Text. Printable ASCII characters. ASCII NUL (0x00) filled on the right, if necessary.
//---------------------------------------------------------------------------------------------------------------------
template<size_t N>
using Text = Alpha<N>;

//---------------------------------------------------------------------------------------------------------------------
// Reserved. Sequence of ASCII NUL (0x00) characters.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<size_t N>
struct Reserved
{
    std::array<uint8_t, N> value;

    Reserved() = default;

    friend std::ostream& operator<<(std::ostream& os, const Reserved& o)
    {
        return os;
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// DateTime. Little Endian, unsigned, eight bytes. Nanoseconds past the UNIX epoch (00:00:00 UTC on 1 January 1970).
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct DateTime
{
    uint64_t value = 0;

    DateTime() = default;

    DateTime(uint64_t v)
      : value(v)
    {
    }

    static DateTime now()
    {
        return { static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
    }

    DateTime operator+(std::chrono::seconds s) const
    {
        return { this->value + std::chrono::duration_cast<std::chrono::nanoseconds>(s).count() };
    }

    DateTime operator+(std::chrono::minutes m) const
    {
        return { this->value + std::chrono::duration_cast<std::chrono::nanoseconds>(m).count() };
    }

    DateTime operator+(std::chrono::hours h) const
    {
        return { this->value + std::chrono::duration_cast<std::chrono::nanoseconds>(h).count() };
    }

    DateTime& operator=(uint64_t v)
    {
        value = v;
        return *this;
    }

    DateTime& operator=(std::string_view v)
    {
        std::from_chars(v.data(), v.data() + v.size(), value);
        return *this;
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Date. Little Endian, unsigned, four bytes. The YYYYMMDD expresses as an integer.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct Date
{
    uint32_t value = 0;

    Date() = default;

    Date(uint32_t v)
      : value(v)
    {
    }

    Date(int y, int m, int d)
      : value(y * 10000 + m * 100 + d)
    {
    }

    Date& operator=(uint32_t v)
    {
        value = v;
        return *this;
    }

    Date& operator=(std::string_view v)
    {
        std::from_chars(v.data(), v.data() + v.size(), value);
        return *this;
    }

    bool operator==(const Date& date) const
    {
        return value == date.value;
    }
};
#pragma pack()

inline std::string
to_string(const Date& o)
{
    static char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%d", o.value);
    buffer[8] = '\0';
    return buffer;
}

template<typename T>
inline T
enum_cast(std::string_view v);

template<typename T>
inline T
enum_cast(int v);

//---------------------------------------------------------------------------------------------------------------------
// AttributedQuote. Allows for an order to be attributed to firm's MPID or RTAL.
//---------------------------------------------------------------------------------------------------------------------
enum class AttributedQuote : char
{
    NotSet          = 0,
    DoNotAttribute  = 'N',
    AttributeMPID   = 'Y',
    AttributeClient = 'C',
    AttributeRTAL   = 'R',
    AttributeBoth   = 'Z'
};

//---------------------------------------------------------------------------------------------------------------------
// BaseLiquidityIndicator. Indicates whether the trader added or removed liquidity.
//---------------------------------------------------------------------------------------------------------------------
enum class BaseLiquidityIndicator : char
{
    Added   = 'A',
    Removed = 'R',
    Routed  = 'X',
    Auction = 'C',
    Waiting = 'W',
};

//---------------------------------------------------------------------------------------------------------------------
// Capacity. The capacity of the order.
//---------------------------------------------------------------------------------------------------------------------
enum class Capacity : char
{
    NotSet            = 0,
    Agency            = 'A',
    Principal         = 'P',
    RisklessPrincipal = 'R',
};

//---------------------------------------------------------------------------------------------------------------------
// CrossTradeFlag. Set the eligibility for periodic auctions.
//---------------------------------------------------------------------------------------------------------------------
enum class CrossTradeFlag : char
{
    None                    = '0',
    PeriodicAuctionOnly     = '1',
    PeriodicAuctionEligible = '2',
    MidPointPeg             = '3'
};

//---------------------------------------------------------------------------------------------------------------------
// DisplayIndicator.
//---------------------------------------------------------------------------------------------------------------------
enum class DisplayIndicator : char
{
    Default                     = 'V',
    PriceAdjust                 = 'P',
    MultiplePriceAdjust         = 'm',
    Cancel                      = 'R',
    Hidden                      = 'r',
    DisplayPriceSliding         = 'S',
    DisplayPriceSlidingIfXNBBO  = 'L',
    MultipleDisplayPriceSliding = 'M',
    VisibleForPegOrders         = 'v',
    Invisible                   = 'I',
    NoRescrapeAtLimit           = 'N',
};

//---------------------------------------------------------------------------------------------------------------------
// ExecInst.
//---------------------------------------------------------------------------------------------------------------------
enum class ExecInst : char
{
    NotSet                          = 0,
    IntermarketSweep                = 'f',
    MarketPeg                       = 'P',
    MarketMakerPeg                  = 'Q',
    PrimaryPeg                      = 'R',
    SuplementalPeg                  = 'U',
    MidpointPeg                     = 'M',
    MidpointPegNotLocked            = 'm',
    AlternateMidpoint               = 'L',
    MidpointDiscretionary           = 'd',
    MidpointDiscretionaryProtection = 'e',
    Late                            = 'r',
    ListingMarketOpening            = 'o',
    ListingMarketClose              = 'c',
    ListingMarketOpeningClose       = 'a',
};

//---------------------------------------------------------------------------------------------------------------------
// ExtExecInst.
//---------------------------------------------------------------------------------------------------------------------
enum class ExtExecInst : char
{
    NotSet                 = 0,
    None                   = 'N',
    Retail                 = 'R',
    RetailPriceImprovement = 'P',
    RetailPriceImproving   = 'T',
    RetailPriority         = 'X',
};

//---------------------------------------------------------------------------------------------------------------------
// ExDestination.
//---------------------------------------------------------------------------------------------------------------------
enum class ExDestination : char
{
    NotSet             = 0,
    NYSEAmerican       = 'A',
    NasdaqTexas        = 'B',
    NYSENational       = 'C',
    TexasStockExchange = 'F',
    X24                = 'G',
    MIAXPearl          = 'H',
    InvestorsExchange  = 'I',
    EDGA               = 'J',
    EDGX               = 'K',
    LTSE               = 'L',
    NYSETexas          = 'M',
    NYSE               = 'N',
    NYSEArca           = 'P',
    Nasdaq             = 'Q',
    IntelligentCross   = 'T',
    MEMX               = 'U',
    NasdaqPSX          = 'X',
    BYX                = 'Y',
    BZX                = 'Z',
};

//---------------------------------------------------------------------------------------------------------------------
// Order Type. Allows for an order to be attributed to firm's MPID or RTAL.
//---------------------------------------------------------------------------------------------------------------------
enum class OrdType : char
{
    Market    = '1',
    Limit     = '2',
    Stop      = '3',
    StopLimit = '4',
    Pegged    = 'P'
};

//---------------------------------------------------------------------------------------------------------------------
// RouteDeliveryMethod.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct RouteDeliveryMethod
{
    Text<3> data = RTI;

    static constexpr const char* RTI = "RTI";
    static constexpr const char* RTF = "RTF";

    RouteDeliveryMethod() = default;

    RouteDeliveryMethod(const char* strategy)
      : data(strategy)
    {
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// RoutingInst.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct RoutingInst
{
    enum class Action : char
    {
        NotSet                = 0,
        BookOnly              = 'B',
        PostOnly              = 'P',
        Routable              = 'R',
        SuperAggressive       = 'S',
        AggressiveCrossOrLock = 'X',
        SuperAggressiveOddLot = 'K',
        PostToAway            = 'A',
        NonDisplayedSwap      = 'N',
    };

    enum class Destination : char
    {
        NotSet           = 0,
        RouteToDRT       = 'D',
        RouteToDisplayed = 'L',
    };

    Action              action      = Action::NotSet;
    Destination         destination = Destination::NotSet;
    std::array<char, 2> padding{ 0 };

    RoutingInst() = default;

    RoutingInst(Action a)
      : action(a)
    {
    }

    RoutingInst(Action a, Destination d)
      : action(a)
      , destination(d)
    {
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// ReplayInstructions. Controls replay behavior for unknown units.
//---------------------------------------------------------------------------------------------------------------------
enum class ReplayInstructions : char
{
    DeepReplay = 'D',
    Fail       = 'F',
    Replay     = 'R',
    Skip       = 'S'
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, ReplayInstructions o)
{
    switch (o) {
            // clang-format off
        case ReplayInstructions::DeepReplay: s << "'D'(DeepReplay)"; break;
        case ReplayInstructions::Fail:       s << "'F'(Fail)";       break;
        case ReplayInstructions::Replay:     s << "'R'(Replay)";     break;
        case ReplayInstructions::Skip:       s << "'S'(Skip)";       break;
        default: s << std::to_string(static_cast<std::underlying_type_t<ReplayInstructions>>(o)) + "(Invalid ReplayInstructions)"; break;
            // clang-format on
    };
    return s;
}

//---------------------------------------------------------------------------------------------------------------------
// Side.
//---------------------------------------------------------------------------------------------------------------------
enum class Side : char
{
    Buy             = '1',
    Sell            = '2',
    SellShort       = '5',
    SellShortExempt = '6'
};

//---------------------------------------------------------------------------------------------------------------------
// SubLiquidityIndicator. Additional information about execution.
//---------------------------------------------------------------------------------------------------------------------
enum class SubLiquidityIndicator : char
{
    NotSet                    = 0,
    AddedRPILiquidity         = 'E',
    AddedHiddenLiquidity      = 'H',
    AddedHiddenPriceImproved  = 'I',
    FirstToJoinNBBO           = 'J',
    PeriodicAuction           = 'P',
    NBBOSetterFeeEligible     = 'S',
    AddedVisiblePriceImproved = 'V',
    MidPintPegOrder           = 'm',
    NBBOSetterNotFeeEligible  = 's',
};

//---------------------------------------------------------------------------------------------------------------------
// Order Type. Allows for an order to be attributed to firm's MPID or RTAL.
//---------------------------------------------------------------------------------------------------------------------
enum class TimeInForce : char
{
    Day        = '0',
    GTC        = '1',
    AtTheOpen  = '2',
    IOC        = '3',
    FOK        = '4',
    GTX        = '5',
    GTD        = '6',
    AtTheClose = '7',
    PRE        = 'E',
    RTP        = 'P',
    RHO        = 'R',
    PTD        = 'T',
    PTX        = 'X',
};

enum class YesNo : char
{
    NotSet = 0,
    Yes    = 'Y',
    No     = 'N'
};

enum class EnabledDisabled : char
{
    NotSet   = 0,
    Disabled = '0',
    Enabled  = '1'
};

//---------------------------------------------------------------------------------------------------------------------
// LoginResponseStatus. Accepted, or the reson for rejection.
//---------------------------------------------------------------------------------------------------------------------
enum class LoginResponseStatus : char
{
    Accepted                 = 'A',
    SessionInUse             = 'B',
    Disabled                 = 'D',
    InvalidUnit              = 'I',
    InvalidMessage           = 'M',
    NotAuthorized            = 'N',
    SequenceAhead            = 'Q',
    ReplayTooDeep            = 'R',
    InvalidSession           = 'S',
    InvalidReplayInstruction = 'X'
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LoginResponseStatus o)
{
    switch (o) {
            // clang-format off
        case LoginResponseStatus::Accepted:                 s << "'A'(Accepted)";                 break;
        case LoginResponseStatus::SessionInUse:             s << "'B'(SessionInUse)";             break;
        case LoginResponseStatus::Disabled:                 s << "'D'(Disabled)";                 break;
        case LoginResponseStatus::InvalidUnit:              s << "'I'(InvalidUnit)";              break;
        case LoginResponseStatus::InvalidMessage:           s << "'M'(InvalidMessage)";           break;
        case LoginResponseStatus::NotAuthorized:            s << "'N'(NotAuthorized)";            break;
        case LoginResponseStatus::SequenceAhead:            s << "'Q'(SequenceAhead)";            break;
        case LoginResponseStatus::ReplayTooDeep:            s << "'R'(ReplayTooDeep)";            break;
        case LoginResponseStatus::InvalidSession:           s << "'S'(InvalidSession)";           break;
        case LoginResponseStatus::InvalidReplayInstruction: s << "'X'(InvalidReplayInstruction)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LoginResponseStatus>>(o)) + "(Invalid LoginResponseStatus)"; break;
            // clang-format on
    };
    return s;
}

//---------------------------------------------------------------------------------------------------------------------
// LogoutReason. Reason for the logout.
//---------------------------------------------------------------------------------------------------------------------
enum class LogoutReason : char
{
    UserRequested     = 'U',
    EndOfDay          = 'E',
    Administrative    = 'A',
    ProtocolViolation = '!'
};

//---------------------------------------------------------------------------------------------------------------------
// Reason Code. These reason codes are used in a variety of contexts - order cancellations and order rejections.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct ReasonCode
{
    Alphanumeric<1> code;

    operator std::string_view() const
    {
        return code.operator std::string_view();
    }

    bool operator==(char c) const
    {
        return *code.begin() == c;
    }

    bool empty() const
    {
        return code.empty();
    }

    const char* to_string() const
    {
        static const std::map<char, const char*> codes = {
            { 'A', "Admin" },
            { 'C', "Capacity undefined" },
            { 'D', "Duplicate identifier (e.g., ClOrdID)" },
            { 'E', "Size reduction due to SWP restatement" },
            { 'F', "Failed to quote" },
            { 'H', "Halted" },
            { 'I', "Incorrect data center" },
            { 'J', "Too late to cancel" },
            { 'K', "Order rate threshold exceeded" },
            { 'L', "Order would lock or cross NBBO" },
            { 'M', "Order size exceeded" },
            { 'N', "Ran out of liquidity to execute against" },
            { 'O', "ClOrdID doesn't match a known order" },
            { 'P', "Can't modify an order that is pending fill" },
            { 'Q', "Waiting for first trade" },
            { 'R', "Routing Unavailable" },
            { 'S', "Short sale price violation" },
            { 'T', "Fill would trade through the NBBO" },
            { 'U', "User requested" },
            { 'V', "Would wash" },
            { 'W', "Add liquidity only order would remove" },
            { 'X', "Order expired" },
            { 'Y', "Symbol not supported" },
            { 'Z', "Unforeseen reason" },
            { 'f', "Risk management MPID or RiskGroupID level" },
            { 'm', "Market access risk limit exceeded" },
            { 'o', "Max open orders count exceeded" },
            { 'r', "Reserve reload" },
            { 's', "Risk management symbol level" },
            { 'u', "Limit Up Limit Down (LULD)" },
            { 'w', "Would remove on unslide" },
            { 'x', "Crossed market" },
            { 'y', "Order received by Cboe during replay" },
            { 'z', "Session end" },
        };

        if (auto itr = codes.find(code[0]); itr != codes.end()) {
            return itr->second;
        }
        return "Unknown reason code";
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// OrderSubReasonCode. These reason codes are used to indicate additional detail for order rejections or cancellations.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct OrderSubReasonCode
{
    Alphanumeric<1> code;

    operator std::string_view() const
    {
        return code.operator std::string_view();
    }

    bool operator==(char c) const
    {
        return *code.begin() == c;
    }

    const char* to_string() const
    {
        static const std::map<char, const char*> codes = {
            { 'A', "Purge/mass cancel EFID level by user" },
            { 'B', "Purge/mass cancel Symbol level by user" },
            { 'C', "Purge/mass cancel Custom Group ID level by user" },
            { 'E', "EFID level lockout by Cboe Trade Desk admin" },
            { 'J', "Firm disconnect" },
            { 'K', "ME disconnect" },
            { 'L', "Unregistered MM Account" },
            { 'S', "Minimum size requirement not met" },
            { 'T', "Cboe Trade Desk admin" },
            { 'f', "Risk management EFID level by rule" },
            { 's', "Risk management Symbol level by rule" },
            { '+', "Risk management EFID Group level by boost::locale::boundary::rule_type" },
        };

        if (auto itr = codes.find(code[0]); itr != codes.end()) {
            return itr->second;
        }
        return "Unknown reason code";
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// PreventMatch.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct PreventMatch
{
    enum class Modifier : char
    {
        NotSet                       = 0,
        CancelNewest                 = 'N',
        CancelOldest                 = 'O',
        CancelBoth                   = 'B',
        CancelSmallest               = 'S',
        DecrementLargerCancelSmaller = 'D',
        DecrementLeaves              = 'd',
    };

    friend std::string to_string(Modifier o)
    {
        switch (o) {
                // clang-format off
            case Modifier::NotSet:                       return "0(NotSet)";                         break;
            case Modifier::CancelNewest:                 return "'N'(CancelNewest)";                 break;
            case Modifier::CancelOldest:                 return "'O'(CancelOldest)";                 break;
            case Modifier::CancelBoth:                   return "'B'(CancelBoth)";                   break;
            case Modifier::CancelSmallest:               return "'S'(CancelSmallest)";               break;
            case Modifier::DecrementLargerCancelSmaller: return "'D'(DecrementLargerCancelSmaller)"; break;
            case Modifier::DecrementLeaves:              return "'d'(DecrementLeaves)";              break;
            default: return std::to_string(static_cast<std::underlying_type_t<Modifier>>(o)) + "(Invalid Modifier)"; break;
                // clang-format on
        };
    }

    enum class Level : char
    {
        NotSet    = 0,
        Firm      = 'F',
        MPID      = 'M',
        Affiliate = 'X',
    };

    friend std::string to_string(Level o)
    {
        switch (o) {
                // clang-format off
            case Level::NotSet:    return "0(NotSet)";      break;
            case Level::Firm:      return "'F'(Firm)";      break;
            case Level::MPID:      return "'M'(MPID)";      break;
            case Level::Affiliate: return "'X'(Affiliate)"; break;
            default: return std::to_string(static_cast<std::underlying_type_t<Level>>(o)) + "(Invalid Level)"; break;
                // clang-format on
        };
    }

    Modifier modifier;
    Level    level;
    char     group = 0;

    PreventMatch() = default;

    PreventMatch(Modifier m, Level l, char g = 0)
      : modifier(m)
      , level(l)
      , group(g)
    {
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// RestatementReason.
//---------------------------------------------------------------------------------------------------------------------
enum class RestatementReason : char
{
    MarketClose         = 'C',
    Reload              = 'L',
    Reprice             = 'P',
    LiquidityUpdate     = 'Q',
    Reroute             = 'R',
    ReductionOfQuantity = 'S',
    Wash                = 'W',
};

//---------------------------------------------------------------------------------------------------------------------
// RoutStrategy.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct RoutStrategy
{
    Text<6> data;

    static constexpr const char* ALLB   = "ALLB";
    static constexpr const char* INET   = "INET";
    static constexpr const char* ICMT   = "ICMT";
    static constexpr const char* IOCM   = "IOCM";
    static constexpr const char* RDOT   = "RDOT";
    static constexpr const char* RDOX   = "RDOX";
    static constexpr const char* ROBB   = "ROBB";
    static constexpr const char* ROCO   = "ROCO";
    static constexpr const char* ROOC   = "ROOC";
    static constexpr const char* ROUD   = "ROUD";
    static constexpr const char* ROLF   = "ROLF";
    static constexpr const char* ROUE   = "ROUE";
    static constexpr const char* ROUQ   = "ROUQ";
    static constexpr const char* ROUT   = "ROUT";
    static constexpr const char* ROUX   = "ROUX";
    static constexpr const char* ROUZ   = "ROUZ";
    static constexpr const char* SWPA   = "SWPA";
    static constexpr const char* SWPB   = "SWPB";
    static constexpr const char* DIRC   = "DIRC";
    static constexpr const char* ROUC   = "ROUC";
    static constexpr const char* RMPT   = "RMPT";
    static constexpr const char* RMPL   = "RMPL";
    static constexpr const char* TRIM   = "TRIM";
    static constexpr const char* TRIMM  = "TRIM-";
    static constexpr const char* SLIM   = "SLIM";
    static constexpr const char* TRIM2  = "TRIM2";
    static constexpr const char* TRIM2M = "TRIM2-";
    static constexpr const char* SLIMP  = "SLIM+";

    // RoutStrategy(std::string_view strategy = "")
    RoutStrategy(const char* strategy = "")
      : data(strategy)
    {
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// FeeCode.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct FeeCode
{
    Alphanumeric<2> code;

    operator std::string_view() const
    {
        return code.operator std::string_view();
    }

    const char& operator[](size_t index) const
    {
        return code.operator[](index);
    }
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
