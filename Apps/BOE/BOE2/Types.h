#ifndef BOE_BOE2_Types_h
#define BOE_BOE2_Types_h

#include "BOE/Utils.h"

#include <algorithm>
#include <concepts>
#include <cmath>
#include <bit>
#include <charconv>
#include <chrono>
#include <type_traits>

namespace BOE::BOE2 {

//---------------------------------------------------------------------------------------------------------------------
// Message types.
//---------------------------------------------------------------------------------------------------------------------
enum class MessageType : uint8_t
{
    LoginRequest    = 0x37,
    LogoutRequest   = 0x02,
    ClientHeartbeat = 0x03,
    NewOrder        = 0x38,
    CancelOrder     = 0x39,
    ModifyOrder     = 0x3A,
    PurgeOrders     = 0x47,

    LoginResponse            = 0x24,
    Logout                   = 0x08,
    ServerHeartbeat          = 0x09,
    ReplayComplete           = 0x13,
    OrderAcknowledgment      = 0x25,
    OrderRejected            = 0x26,
    OrderModified            = 0x27,
    OrderRestated            = 0x28,
    UserModifyRejected       = 0x29,
    OrderCancelled           = 0x2A,
    CancelRejected           = 0x2B,
    OrderExecution           = 0x2C,
    TradeCancelOrCorrect     = 0x2D,
    MassCancelAcknowledgment = 0x36,
    PurgeRejected            = 0x48,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, MessageType o)
{
    switch (o) {
            // clang-format off
        case MessageType::LoginRequest:             s << "55(LoginRequest)";             break;
        case MessageType::LogoutRequest:            s << "2(LogoutRequest)";             break;
        case MessageType::ClientHeartbeat:          s << "3(ClientHeartbeat)";           break;
        case MessageType::NewOrder:                 s << "56(NewOrder)";                 break;
        case MessageType::CancelOrder:              s << "57(CancelOrder)";              break;
        case MessageType::ModifyOrder:              s << "58(ModifyOrder)";              break;
        case MessageType::PurgeOrders:              s << "71(PurgeOrders)";              break;
        case MessageType::LoginResponse:            s << "36(LoginResponse)";            break;
        case MessageType::Logout:                   s << "8(Logout)";                    break;
        case MessageType::ServerHeartbeat:          s << "9(ServerHeartbeat)";           break;
        case MessageType::ReplayComplete:           s << "19(ReplayComplete)";           break;
        case MessageType::OrderAcknowledgment:      s << "37(OrderAcknowledgment)";      break;
        case MessageType::OrderRejected:            s << "38(OrderRejected)";            break;
        case MessageType::OrderModified:            s << "39(OrderModified)";            break;
        case MessageType::OrderRestated:            s << "40(OrderRestated)";            break;
        case MessageType::UserModifyRejected:       s << "41(UserModifyRejected)";       break;
        case MessageType::OrderCancelled:           s << "42(OrderCancelled)";           break;
        case MessageType::CancelRejected:           s << "43(CancelRejected)";           break;
        case MessageType::OrderExecution:           s << "44(OrderExecution)";           break;
        case MessageType::TradeCancelOrCorrect:     s << "45(TradeCancelOrCorrect)";     break;
        case MessageType::MassCancelAcknowledgment: s << "54(MassCancelAcknowledgment)"; break;
        case MessageType::PurgeRejected:            s << "72(PurgeRejected)";            break;
        default: s << std::to_string(static_cast<std::underlying_type_t<MessageType>>(o)) + "(Invalid MessageTypes)"; break;
            // clang-format on
    };
    return s;
}

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
// SignedBinary. Little Endian byte order, signed binary value. The number of bytes used depends on the context.
//---------------------------------------------------------------------------------------------------------------------
template<size_t N>
struct SignedBinary;

#pragma pack(1)
template<>
struct SignedBinary<1>
{
    int8_t value = 0;

    SignedBinary() = default;

    SignedBinary(int8_t v)
      : value(v)
    {
    }

    operator int8_t() const
    {
        return value;
    }

    int8_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const SignedBinary& o)
    {
        return os << unsigned(o.value);
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct SignedBinary<2>
{
    int16_t value = 0;

    SignedBinary() = default;

    SignedBinary(int16_t v)
      : value(v)
    {
    }

    operator int16_t() const
    {
        return value;
    }

    int16_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const SignedBinary& o)
    {
        return os << o.value;
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct SignedBinary<4>
{
    int32_t value = 0;

    SignedBinary() = default;

    SignedBinary(int32_t v)
      : value(v)
    {
    }

    operator int32_t() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const SignedBinary& o)
    {
        return os << o.value;
    }
};
#pragma pack()

#pragma pack(1)
template<>
struct SignedBinary<8>
{
    int64_t value = 0;

    SignedBinary() = default;

    SignedBinary(int64_t v)
      : value(v)
    {
    }

    operator int64_t() const
    {
        return value;
    }

    int64_t v() const
    {
        return value;
    }

    friend std::ostream& operator<<(std::ostream& os, const SignedBinary& o)
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

    bool operator==(std::string_view v) const
    {
        return v == static_cast<std::string_view>(*this);
    }

    bool operator!=(std::string_view v) const
    {
        return not operator==(v);
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

//---------------------------------------------------------------------------------------------------------------------
// Woptional Field. Holds data and bitfield position as type attributes.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<uint8_t BYTE, uint8_t BIT, typename V>
struct OptionalField
{
    enum
    {
        Byte = BYTE
    };
    enum
    {
        Bit = BIT
    };

    V value;

    OptionalField(V&& v)
      : value(v)
    {
    }

    OptionalField(const OptionalField& a) = default;
    OptionalField(OptionalField&& a)      = default;

    static std::size_t size()
    {
        return sizeof(OptionalField);
    }
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Side
//---------------------------------------------------------------------------------------------------------------------
enum class Side : uint8_t
{
    Buy             = '1',
    Sell            = '2',
    SellShort       = '5',
    SellShortExempt = '6',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, Side o)
{
    switch (o) {
            // clang-format off
        case Side::Buy:             s << "'1'(Buy)";             break;
        case Side::Sell:            s << "'2'(Sell)";            break;
        case Side::SellShort:       s << "'5'(SellShort)";       break;
        case Side::SellShortExempt: s << "'6'(SellShortExempt)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<Side>>(o)) + "(Invalid Side)"; break;
            // clang-format on
    };
    return s;
}

enum class ReasonCodes : uint8_t
{
    Admin                                = 'A',
    CapacityUndefined                    = 'C',
    DuplicateIdentifier                  = 'D',
    SizeReductionDueToSwpRestatement     = 'E',
    FailedToQuote                        = 'F',
    Halted                               = 'H',
    IncorrectDataCenter                  = 'I',
    TooLateToCancel                      = 'J',
    OrderRateThresholdExceeded           = 'K',
    OrderWouldLockOrCrossNbbo            = 'L',
    OrderSizeExceeded                    = 'M',
    RanOutOfLiquidityToExecuteAgainst    = 'N',
    ClordidDoesntMatchAKnownOrder        = 'O',
    CantModifyAnOrderThatIsPendingFill   = 'P',
    WaitingForFirstTrade                 = 'Q',
    RoutingUnavailable                   = 'R',
    ShortSalePriceViolation              = 'S',
    UserRequested                        = 'U',
    WouldWash                            = 'V',
    AddLiquidityOnlyOrderWouldRemove     = 'W',
    OrderExpired                         = 'X',
    SymbolNotSupported                   = 'Y',
    UnforeseenReason                     = 'Z',
    RiskManagementMpidOrRiskgroupidLevel = 'f',
    MarketAccessRiskLimitExceeded        = 'm',
    MaxOpenOrdersCountExceeded           = 'o',
    ReserveReload                        = 'r',
    RiskManagementSymbolLevel            = 's',
    LimitUpLimitDown                     = 'u',
    WouldRemoveOnUnslide                 = 'w',
    CrossedMarket                        = 'x',
    OrderReceivedByCboeDuringReplay      = 'y',
};

/*
XXXX
//---------------------------------------------------------------------------------------------------------------------
// Party Role Qualifier.
//---------------------------------------------------------------------------------------------------------------------
enum class ClientIdentification : uint8_t
{
    None    = 0,
    Firm    = 1,
    Natural = 3,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, ClientIdentification o)
{
    switch (o) {
            // clang-format off
        case ClientIdentification::None:    s << "0(None)";    break;
        case ClientIdentification::Firm:    s << "1(Firm)";    break;
        case ClientIdentification::Natural: s << "3(Natural)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<ClientIdentification>>(o)) + "(Invalid ClientIdentification)"; break;
            // clang-format on
    };
    return s;
}

enum class InvestmentDecisionMaker : uint8_t
{
    None    = 0,
    Algo    = 2,
    Natural = 3,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, InvestmentDecisionMaker o)
{
    switch (o) {
            // clang-format off
        case InvestmentDecisionMaker::None:    s << "0(None)";    break;
        case InvestmentDecisionMaker::Algo:    s << "2(Algo)";    break;
        case InvestmentDecisionMaker::Natural: s << "3(Natural)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<InvestmentDecisionMaker>>(o)) + "(Invalid InvestmentDecisionMaker)"; break;
            // clang-format on
    };
    return s;
}

enum class ExecutionDecisionWithinFirm : uint8_t
{
    None    = 0,
    Algo    = 2,
    Natural = 3,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, ExecutionDecisionWithinFirm o)
{
    switch (o) {
            // clang-format off
        case ExecutionDecisionWithinFirm::None:    s << "0(None)";    break;
        case ExecutionDecisionWithinFirm::Algo:    s << "2(Algo)";    break;
        case ExecutionDecisionWithinFirm::Natural: s << "3(Natural)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<ExecutionDecisionWithinFirm>>(o)) + "(Invalid ExecutionDecisionWithinFirm)"; break;
            // clang-format on
    };
    return s;
}

#pragma pack(1)
struct PartyRoleQualifier
{
    uint8_t value = 0;

    PartyRoleQualifier(ClientIdentification        c = ClientIdentification::None,
                       InvestmentDecisionMaker     i = InvestmentDecisionMaker::None,
                       ExecutionDecisionWithinFirm e = ExecutionDecisionWithinFirm::None)
      : value(std::underlying_type_t<ClientIdentification>(c) | (std::underlying_type_t<InvestmentDecisionMaker>(i) << 2) |
              (std::underlying_type_t<ExecutionDecisionWithinFirm>(e) << 4))
    {
    }

    explicit operator ClientIdentification() const
    {
        return ClientIdentification(value & 0x03);
    }

    explicit operator InvestmentDecisionMaker() const
    {
        return InvestmentDecisionMaker((value >> 2) & 0x03);
    }

    explicit operator ExecutionDecisionWithinFirm() const
    {
        return ExecutionDecisionWithinFirm((value >> 4) & 0x03);
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const PartyRoleQualifier& o)
{
    // clang-format off
    s << "[PartyRoleQualifier]=";
    s << "ClientIdentification: "        << ClientIdentification(o)    << ' ';
    s << "InvestmentDecisionMaker: "     << InvestmentDecisionMaker(o) << ' ';
    s << "ExecutionDecisionWithinFirm: " << ExecutionDecisionWithinFirm(o);
    // clang-format on
    return s;
}

//---------------------------------------------------------------------------------------------------------------------
// Liquidity Attributes.
//---------------------------------------------------------------------------------------------------------------------
enum class LiquidityIndicator : uint8_t
{
    Added    = 0,
    Removed  = 1,
    Auction  = 2,
    Reserved = 3,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LiquidityIndicator o)
{
    switch (o) {
            // clang-format off
        case LiquidityIndicator::Added:    s << "0(Added)";    break;
        case LiquidityIndicator::Removed:  s << "1(Removed)";  break;
        case LiquidityIndicator::Auction:  s << "2(Auction)";  break;
        case LiquidityIndicator::Reserved: s << "3(Reserved)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LiquidityIndicator>>(o)) + "(Invalid LiquidityIndicator)"; break;
            // clang-format on
    };
    return s;
}

enum class LiquidityInternalized : uint8_t
{
    NotInternalized = 0,
    Internalized    = 1,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LiquidityInternalized o)
{
    switch (o) {
            // clang-format off
        case LiquidityInternalized::NotInternalized: s << "0(NotInternalized)"; break;
        case LiquidityInternalized::Internalized:    s << "1(Internalized)";    break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LiquidityInternalized>>(o)) + "(Invalid LiquidityInternalized)"; break;
            // clang-format on
    };
    return s;
}

enum class LiquidityTopOfBook : uint8_t
{
    NotTopOfBook = 0,
    TopOfBook    = 1,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LiquidityTopOfBook o)
{
    switch (o) {
            // clang-format off
        case LiquidityTopOfBook::NotTopOfBook: s << "0(NotTopOfBook)"; break;
        case LiquidityTopOfBook::TopOfBook:    s << "1(TopOfBook)";    break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LiquidityTopOfBook>>(o)) + "(Invalid LiquidityTopOfBook)"; break;
            // clang-format on
    };
    return s;
}

enum class LiquiditySelfTrade : uint8_t
{
    NotSelfTrade = 0,
    SelfTrade    = 1,
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LiquiditySelfTrade o)
{
    switch (o) {
            // clang-format off
        case LiquiditySelfTrade::NotSelfTrade: s << "0(NotSelfTrade)"; break;
        case LiquiditySelfTrade::SelfTrade:    s << "1(SelfTrade)";    break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LiquiditySelfTrade>>(o)) + "(Invalid LiquiditySelfTrade)"; break;
            // clang-format on
    };
    return s;
}

#pragma pack(1)
struct LiquidityAttributes
{
    uint8_t value = 0;

    explicit LiquidityAttributes(LiquidityIndicator    i = LiquidityIndicator::Added,
                                 LiquidityInternalized n = LiquidityInternalized::NotInternalized,
                                 LiquidityTopOfBook    t = LiquidityTopOfBook::NotTopOfBook,
                                 LiquiditySelfTrade    s = LiquiditySelfTrade::NotSelfTrade)
      : value((std::underlying_type_t<LiquidityIndicator>(i) << 3) | (std::underlying_type_t<LiquidityInternalized>(n) << 5) |
              (std::underlying_type_t<LiquidityTopOfBook>(t) << 6) | (std::underlying_type_t<LiquiditySelfTrade>(s) << 7))
    {
    }

    explicit operator LiquidityIndicator() const
    {
        return LiquidityIndicator((value >> 3) & 0x03);
    }

    explicit operator LiquidityInternalized() const
    {
        return LiquidityInternalized((value >> 5) & 0x01);
    }

    explicit operator LiquidityTopOfBook() const
    {
        return LiquidityTopOfBook((value >> 6) & 0x01);
    }

    explicit operator LiquiditySelfTrade() const
    {
        return LiquiditySelfTrade((value >> 7) & 0x01);
    }
};
#pragma pack()

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, const LiquidityAttributes& o)
{
    // clang-format off
    s << "[LiquidityAttributes]=";
    s << "LiquidityIndicator: "    << LiquidityIndicator(o)    << ' ';
    s << "LiquidityInternalized: " << LiquidityInternalized(o) << ' ';
    s << "LiquidityTopOfBook: "    << LiquidityTopOfBook(o)    << ' ';
    s << "LiquiditySelfTrade: "    << LiquiditySelfTrade(o);
    // clang-format on
    return s;
}

//---------------------------------------------------------------------------------------------------------------------
// User Reference Number. Unsigned int.
//---------------------------------------------------------------------------------------------------------------------
using UserRefNum = UInt32;

template<typename T>
static T
enum_cast(std::string_view e);

enum class BuySellIndicator : uint8_t
{
    Buy  = 'B',
    Sell = 'S',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, BuySellIndicator o)
{
    switch (o) {
            // clang-format off
        case BuySellIndicator::Buy:  s << "'B'(Buy)";  break;
        case BuySellIndicator::Sell: s << "'S'(Sell)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<BuySellIndicator>>(o)) + "(Invalid BuySellIndicator)"; break;
            // clang-format on
    };
    return s;
}

template<>
inline BuySellIndicator
enum_cast(std::string_view e)
{
    // clang-format off
    if (e == "1") return BuySellIndicator::Buy;
    if (e == "2") return BuySellIndicator::Sell;
    // clang-format on
    throw std::out_of_range("Value " + std::string(e) + " is outside of BuySellIndicator enumeration range.");
}

enum class EventCode : uint8_t
{
    StartOfDay = 'S',
    EndOfDay   = 'E',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, EventCode o)
{
    switch (o) {
            // clang-format off
        case EventCode::StartOfDay: s << "'S'(StartOfDay)"; break;
        case EventCode::EndOfDay:   s << "'E'(EndOfDay)";   break;
        default: s << std::to_string(static_cast<std::underlying_type_t<EventCode>>(o)) + "(Invalid EventCode)"; break;
            // clang-format on
    };
    return s;
}

enum class Capacity : uint8_t
{
    Client                    = '1',
    OwnAccount                = '2',
    MarketMaker               = '3',
    IssuerHoldingAOTC         = '4',
    IssuePriceStabilizingAOTC = '6',
    RisklessPrincipal         = '7',
    IssuerHoldingDEAL         = '8',
    IssuePriceStabilizingDEAL = '9',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, Capacity o)
{
    switch (o) {
            // clang-format off
        case Capacity::Client:                    s << "'1'(Client)";                    break;
        case Capacity::OwnAccount:                s << "'2'(OwnAccount)";                break;
        case Capacity::MarketMaker:               s << "'3'(MarketMaker)";               break;
        case Capacity::IssuerHoldingAOTC:         s << "'4'(IssuerHoldingAOTC)";         break;
        case Capacity::IssuePriceStabilizingAOTC: s << "'6'(IssuePriceStabilizingAOTC)"; break;
        case Capacity::RisklessPrincipal:         s << "'7'(RisklessPrincipal)";         break;
        case Capacity::IssuerHoldingDEAL:         s << "'8'(IssuerHoldingDEAL)";         break;
        case Capacity::IssuePriceStabilizingDEAL: s << "'9'(IssuePriceStabilizingDEAL)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<Capacity>>(o)) + "(Invalid Capacity)"; break;
            // clang-format on
    };
    return s;
}

enum class TransactionCategory : uint8_t
{
    DarkTrade = 'D',
    NoneApply = '-',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, TransactionCategory o)
{
    switch (o) {
            // clang-format off
        case TransactionCategory::DarkTrade: s << "'D'(DarkTrade)"; break;
        case TransactionCategory::NoneApply: s << "'-'(NoneApply)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<TransactionCategory>>(o)) + "(Invalid TransactionCategory)"; break;
            // clang-format on
    };
    return s;
}

enum class AlgoIndicator : uint8_t
{
    NoAlgo = '-',
    Algo   = 'H',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, AlgoIndicator o)
{
    switch (o) {
            // clang-format off
        case AlgoIndicator::NoAlgo: s << "'-'(NoAlgo)"; break;
        case AlgoIndicator::Algo:   s << "'H'(Algo)";   break;
        default: s << std::to_string(static_cast<std::underlying_type_t<AlgoIndicator>>(o)) + "(Invalid AlgoIndicator)"; break;
            // clang-format on
    };
    return s;
}

enum class PegType : uint8_t
{
    Midpoint = 'M',
    Market   = 'P',
    Primary  = 'R',
};

enum class TimeInForce : uint8_t
{
    DAY = '0',
    GTC = '1',
    IOC = '3',
    GTT = '6',
    GFA = 'B',
};

template<>
inline TimeInForce
enum_cast(std::string_view e)
{
    // clang-format off
    if (e == "0") return TimeInForce::DAY;
    if (e == "1") return TimeInForce::GTC;
    if (e == "3") return TimeInForce::IOC;
    if (e == "6") return TimeInForce::GTT;
    if (e == "B") return TimeInForce::GFA;
    // clang-format on
    throw std::out_of_range("Value " + std::string(e) + " is outside of TimeInForce enumeration range.");
}

enum class ClearingAccountType : uint8_t
{
    Client = '1',
    House  = '2',
};

enum class CrossType : uint8_t
{
    Auction  = 'A',
    Opening  = 'O',
    Intraday = 'I',
    Closing  = 'C',
    Halt     = 'H',
};

enum class LiquidityFlag : uint8_t
{
    ContinuousMarketTrade = 'A',
    AuctionTrade          = 'C',
    AuctionOnDemandTrade  = 'P',
    TradingAtClosePrice   = 'G',
    NordicAtMid           = 'M',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, LiquidityFlag o)
{
    switch (o) {
            // clang-format off
        case LiquidityFlag::ContinuousMarketTrade: s << "'A'(ContinuousMarketTrade)"; break;
        case LiquidityFlag::AuctionTrade:          s << "'C'(AuctionTrade)";          break;
        case LiquidityFlag::AuctionOnDemandTrade:  s << "'P'(AuctionOnDemandTrade)";  break;
        case LiquidityFlag::TradingAtClosePrice:   s << "'G'(TradingAtClosePrice)";   break;
        case LiquidityFlag::NordicAtMid:           s << "'M'(NordicAtMid)";           break;
        default: s << std::to_string(static_cast<std::underlying_type_t<LiquidityFlag>>(o)) + "(Invalid LiquidityFlag)"; break;
            // clang-format on
    };
    return s;
}

enum class TradingMode : uint8_t
{
    ScheduledOpeningAuction  = 'O',
    ScheduledClosingAuction  = 'K',
    ScheduledIntradayAuction = 'I',
    UnscheduledAuction       = 'U',
    ContinuousTrading        = '2',
    AtMarketCloseTrading     = '3',
    OnDemandAuction          = 'P',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, TradingMode o)
{
    switch (o) {
            // clang-format off
        case TradingMode::ScheduledOpeningAuction:  s << "'O'(ScheduledOpeningAuction)";  break;
        case TradingMode::ScheduledClosingAuction:  s << "'K'(ScheduledClosingAuction)";  break;
        case TradingMode::ScheduledIntradayAuction: s << "'I'(ScheduledIntradayAuction)"; break;
        case TradingMode::UnscheduledAuction:       s << "'U'(UnscheduledAuction)";       break;
        case TradingMode::ContinuousTrading:        s << "'2'(ContinuousTrading)";        break;
        case TradingMode::AtMarketCloseTrading:     s << "'3'(AtMarketCloseTrading)";     break;
        case TradingMode::OnDemandAuction:          s << "'P'(OnDemandAuction)";          break;
        default: s << std::to_string(static_cast<std::underlying_type_t<TradingMode>>(o)) + "(Invalid TradingMode)"; break;
            // clang-format on
    };
    return s;
}

enum class DEAIndicator : uint8_t
{
    Customer                = '1',
    Firm                    = '2',
    Broker                  = '3',
    CustomerOrFirm          = '4',
    DirectOrSponcoredAccess = '5',
};

enum class Display : uint8_t
{
    Displayed   = 'Y',
    Hidden      = 'N',
    Auction     = 'A',
    NordicAtMid = 'M',
};

enum class LiquidityProvisionIndicator : uint8_t
{
    NoLiquidityProvision = 'N',
    LiquidityProvision   = 'Y',
};

enum class STPAction : uint8_t
{
    CancelPassiveOrder        = '1',
    CancelAggressiveOrder     = '2',
    CancelBothOrders          = '3',
    CreateTransferTransaction = '4',
};

enum class STPLevel : uint8_t
{
    MPIDPlusTRader = '1',
    MPID           = '2',
    TraderGroup    = '3',
};

enum class OrderCondition : uint8_t
{
    MarketMakerOrder   = 'W',
    MarketMakerRefresh = 'U',
    TopOfBook          = 'P',
    DarkLitSweep       = 'Q',
    TradeNow           = 'T',
};

enum class PendingReason : uint8_t
{
    CompletionOfAuctionOfDemand = 'A',
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, PendingReason o)
{
    switch (o) {
            // clang-format off
        case PendingReason::CompletionOfAuctionOfDemand: s << "'A'(CompletionOfAuctionOfDemand)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<PendingReason>>(o)) + "(Invalid PendingReason)"; break;
            // clang-format on
    };
    return s;
}

#pragma pack(1)
struct RejectReason
{
    UInt16 value = 0;

    RejectReason(int code = 0)
      : value(static_cast<uint16_t>(code))
    {
    }

    static std::string getRejectMessage(int code)
    {
        return "Unknown code: " + std::to_string(code);
    }

    friend std::ostream& operator<<(std::ostream& os, const RejectReason& o)
    {
        return os << '(' + std::to_string(o.value) + ')' + getRejectMessage(o.value);
    }
};
#pragma pack()

inline std::string
to_string(const RejectReason& r)
{
    return RejectReason::getRejectMessage(r.value);
}

enum class Market : uint8_t
{
    DCSE      = 1,   // NASDAQ COPENHAGEN A/S - NORDIC@MID
    MCSE      = 2,   // NASDAQ COPENHAGEN A/S - AUCTION ON DEMAND
    XCSE      = 3,   // NASDAQ COPENHAGEN A/S
    XTAL      = 4,   // NASDAQ TALLINN AS
    DHEL      = 5,   // NASDAQ HELSINKI LTD - NORDIC@MID
    MHEL      = 6,   // NASDAQ HELSINKI LTD - AUCTION ON DEMAND
    XHEL      = 7,   // NASDAQ HELSINKI LTD
    DICE      = 8,   // NASDAQ ICELAND HF. - NORDIC@MID
    MICE      = 9,   // NASDAQ ICELAND HF. - AUCTION ON DEMAND
    XICE      = 10,  // NASDAQ ICELAND HF.
    XRIS      = 11,  // NASDAQ RIGA AS
    XLIT      = 12,  // AB NASDAQ VILNIUS
    DSTO      = 14,  // NASDAQ STOCKHOLM AB - NORDIC@MID
    ESTO      = 15,  // NASDAQ STOCKHOLM AB - NORWAY ETF
    MSTO      = 16,  // NASDAQ STOCKHOLM AB - AUCTION ON DEMAND
    XSTO      = 17,  // NASDAQ STOCKHOLM AB
    DNDK      = 18,  // FIRST NORTH DENMARK - NORDIC@MID
    DSME      = 19,  // FIRST NORTH DENMARK - SME GROWTH MARKET
    FNDK      = 20,  // FIRST NORTH DENMARK
    MNDK      = 21,  // FIRST NORTH DENMARK - AUCTION ON DEMAND
    FNEE      = 22,  // FIRST NORTH ESTONIA
    DNFI      = 23,  // FIRST NORTH FINLAND - NORDIC@MID
    FNFI      = 24,  // FIRST NORTH FINLAND
    FSME      = 25,  // FIRST NORTH FINLAND - SME GROWTH MARKET
    MNFI      = 26,  // FIRST NORTH FINLAND - AUCTION ON DEMAND
    DNIS      = 27,  // FIRST NORTH ICELAND - NORDIC@MID
    FNIS      = 28,  // FIRST NORTH ICELAND
    MNIS      = 29,  // FIRST NORTH ICELAND - AUCTION ON DEMAND
    FNLV      = 30,  // FIRST NORTH LATVIA
    FNLT      = 31,  // FIRST NORTH LITHUANIA
    DNSE      = 32,  // FIRST NORTH SWEDEN - NORDIC@MID
    DOSE      = 33,  // FIRST NORTH SWEDEN - NORWAY NORDIC@MID
    FNSE      = 34,  // FIRST NORTH SWEDEN
    MNSE      = 35,  // FIRST NORTH SWEDEN - AUCTION ON DEMAND
    MOSE      = 36,  // FIRST NORTH SWEDEN - NORWAY AUCTION ON DEMAND
    ONSE      = 37,  // FIRST NORTH SWEDEN - NORWAY
    SSME      = 38,  // FIRST NORTH SWEDEN - SME GROWTH MARKET
    XSAT      = 39,  // SPOTLIGHT STOCK MARKET AB
    SPDK      = 40,  // SPOTLIGHT STOCK MARKET DENMARK
    SPFI      = 41,  // SPOTLIGHT STOCK MARKET FINLAND
    SPNO      = 42,  // SPOTLIGHT STOCK MARKET NORWAY
    SPSD      = 48,  // SPOTLIGHT STOCK MARKET ETP
    Undefined = 255, // MIC of the execution is one not currently defined in this specification.
};

inline [[cppgen::auto]] std::ostream&
operator<<(std::ostream& s, Market o)
{
    switch (o) {
            // clang-format off
        case Market::DCSE:      s << "1(DCSE)";        break;
        case Market::MCSE:      s << "2(MCSE)";        break;
        case Market::XCSE:      s << "3(XCSE)";        break;
        case Market::XTAL:      s << "4(XTAL)";        break;
        case Market::DHEL:      s << "5(DHEL)";        break;
        case Market::MHEL:      s << "6(MHEL)";        break;
        case Market::XHEL:      s << "7(XHEL)";        break;
        case Market::DICE:      s << "8(DICE)";        break;
        case Market::MICE:      s << "9(MICE)";        break;
        case Market::XICE:      s << "10(XICE)";       break;
        case Market::XRIS:      s << "11(XRIS)";       break;
        case Market::XLIT:      s << "12(XLIT)";       break;
        case Market::DSTO:      s << "14(DSTO)";       break;
        case Market::ESTO:      s << "15(ESTO)";       break;
        case Market::MSTO:      s << "16(MSTO)";       break;
        case Market::XSTO:      s << "17(XSTO)";       break;
        case Market::DNDK:      s << "18(DNDK)";       break;
        case Market::DSME:      s << "19(DSME)";       break;
        case Market::FNDK:      s << "20(FNDK)";       break;
        case Market::MNDK:      s << "21(MNDK)";       break;
        case Market::FNEE:      s << "22(FNEE)";       break;
        case Market::DNFI:      s << "23(DNFI)";       break;
        case Market::FNFI:      s << "24(FNFI)";       break;
        case Market::FSME:      s << "25(FSME)";       break;
        case Market::MNFI:      s << "26(MNFI)";       break;
        case Market::DNIS:      s << "27(DNIS)";       break;
        case Market::FNIS:      s << "28(FNIS)";       break;
        case Market::MNIS:      s << "29(MNIS)";       break;
        case Market::FNLV:      s << "30(FNLV)";       break;
        case Market::FNLT:      s << "31(FNLT)";       break;
        case Market::DNSE:      s << "32(DNSE)";       break;
        case Market::DOSE:      s << "33(DOSE)";       break;
        case Market::FNSE:      s << "34(FNSE)";       break;
        case Market::MNSE:      s << "35(MNSE)";       break;
        case Market::MOSE:      s << "36(MOSE)";       break;
        case Market::ONSE:      s << "37(ONSE)";       break;
        case Market::SSME:      s << "38(SSME)";       break;
        case Market::XSAT:      s << "39(XSAT)";       break;
        case Market::SPDK:      s << "40(SPDK)";       break;
        case Market::SPFI:      s << "41(SPFI)";       break;
        case Market::SPNO:      s << "42(SPNO)";       break;
        case Market::SPSD:      s << "48(SPSD)";       break;
        case Market::Undefined: s << "255(Undefined)"; break;
        default: s << std::to_string(static_cast<std::underlying_type_t<Market>>(o)) + "(Invalid Market)"; break;
            // clang-format on
    };
    return s;
}
*/

} // namespace BOE::BOE2

#endif
