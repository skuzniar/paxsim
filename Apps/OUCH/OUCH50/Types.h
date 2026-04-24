#ifndef OUCH_OUCH50_Types_h
#define OUCH_OUCH50_Types_h

#include "OUCH/Utils.h"

#include <algorithm>
#include <concepts>
#include <cmath>
#include <bit>
#include <charconv>
#include <chrono>
#include <type_traits>

namespace OUCH::OUCH50 {

#pragma pack(1)
template<std::integral T>
struct big_endian
{
    big_endian() = default;

    big_endian(T value)
    {
        if constexpr (std::endian::native != std::endian::big) {
            if constexpr (sizeof(T) == sizeof(uint8_t)) {
                m_value = value;
            } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                m_value = __builtin_bswap16(static_cast<uint16_t>(value));
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                m_value = __builtin_bswap32(static_cast<uint32_t>(value));
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                m_value = __builtin_bswap64(static_cast<uint64_t>(value));
            } else {
                static_assert(sizeof(T), "Unsupported type");
            }
        } else {
            m_value = value;
        }
    }

    big_endian(std::string_view v)
    {
        T value;
        std::from_chars(v.data(), v.data() + v.size(), value);
        *this = value;
    }

    operator T() const
    {
        if constexpr (std::endian::native != std::endian::big) {
            if constexpr (sizeof(T) == sizeof(uint8_t)) {
                return m_value;
            } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                return static_cast<T>(__builtin_bswap16(m_value));
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                return static_cast<T>(__builtin_bswap32(m_value));
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                return static_cast<T>(__builtin_bswap64(m_value));
            } else {
                static_assert(sizeof(T), "Unsupported type");
            }
        } else {
            return m_value;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, big_endian<T> value)
    {
        return os << static_cast<T>(value);
    }

private:
    std::make_unsigned_t<T> m_value{ 0 };
};

template<std::integral T>
struct little_endian
{
    little_endian() = default;

    little_endian(T value)
    {
        if constexpr (std::endian::native != std::endian::little) {
            if constexpr (sizeof(T) == sizeof(uint8_t)) {
                m_value = value;
            } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                m_value = __builtin_bswap16(static_cast<uint16_t>(value));
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                m_value = __builtin_bswap32(static_cast<uint32_t>(value));
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                m_value = __builtin_bswap64(static_cast<uint64_t>(value));
            } else {
                static_assert(sizeof(T), "Unsupported type");
            }
        } else {
            m_value = value;
        }
    }

    little_endian(std::string_view v)
    {
        T value;
        std::from_chars(v.data(), v.data() + v.size(), value);
        *this = value;
    }

    operator T() const
    {
        if constexpr (std::endian::native != std::endian::little) {
            if constexpr (sizeof(T) == sizeof(uint8_t)) {
                return m_value;
            } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
                return static_cast<T>(__builtin_bswap16(m_value));
            } else if constexpr (sizeof(T) == sizeof(uint32_t)) {
                return static_cast<T>(__builtin_bswap32(m_value));
            } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
                return static_cast<T>(__builtin_bswap64(m_value));
            } else {
                static_assert(sizeof(T), "Unsupported type");
            }
        } else {
            return m_value;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, little_endian<T> value)
    {
        return os << static_cast<T>(value);
    }

private:
    std::make_unsigned_t<T> m_value{ 0 };
};
#pragma pack()

//---------------------------------------------------------------------------------------------------------------------
// Alpha field. Left justified and padded on the right with spaces.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
template<size_t N>
struct Alpha
{
    std::array<char, N> value{};

    Alpha()
    {
        std::fill(value.data(), value.end(), ' ');
    }

    Alpha(std::string_view v)
    {
        std::size_t len = std::min(value.size(), v.size());
        std::copy(v.data(), v.data() + len, value.data());
        std::fill(value.data() + len, value.end(), ' ');
    }

    template<typename I>
    Alpha(I v)
    {
        ritoa(value.data(), value.size(), v);
    }

    template<size_t K>
    Alpha(const char (&array)[K])
      : Alpha(std::string_view(std::addressof(array[0]), strnlen(std::addressof(array[0]), K)))
    {
    }

    Alpha& operator=(std::string_view v)
    {
        std::size_t len = std::min(value.size(), v.size());
        std::copy(v.data(), v.data() + len, value.data());
        std::fill(value.data() + len, value.end(), ' ');
        return *this;
    }

    char* data()
    {
        return value.data();
    }

    const char* data() const
    {
        return value.data();
    }

    std::size_t size() const
    {
        return value.size();
    }

    // Contrary to what the specification says, we get null characters in Alpha fields.
    operator std::string_view() const
    {
        static std::array<char, 2> tc = { ' ', '\0' };
        static std::string_view    tv = { tc.data(), tc.size() };

        std::string_view sv = { value.data(), value.size() };
        auto             tp = sv.find_last_not_of(tv);
        tp                  = tp == std::string_view::npos ? 0 : tp + 1;
        return value[0] == 0 ? std::string_view() : sv.substr(0, tp);
    }

    bool operator==(std::string_view v) const
    {
        return v == static_cast<std::string_view>(*this);
    }
    bool operator!=(std::string_view v) const
    {
        return not operator==(v);
    }

    friend std::ostream& operator<<(std::ostream& os, const Alpha& a)
    {
        return os << std::string_view(a);
    }

    friend std::string to_json(const Alpha& a, bool)
    {
        return '"' + std::string(std::string_view(a)) + '"';
    }
};
#pragma pack()

template<size_t N>
std::string
to_string(const Alpha<N>& a)
{
    return std::string(std::string_view(a));
}

using UInt8  = big_endian<uint8_t>;
using UInt16 = big_endian<uint16_t>;
using UInt32 = big_endian<uint32_t>;
using UInt64 = big_endian<uint64_t>;

using Int8  = big_endian<int8_t>;
using Int16 = big_endian<int16_t>;
using Int32 = big_endian<int32_t>;
using Int64 = big_endian<int64_t>;

//---------------------------------------------------------------------------------------------------------------------
// Price is a four byte integer, fixed format with six whole digits and four decimal digits.
//---------------------------------------------------------------------------------------------------------------------
struct Price
{
    static const long multiplier = 10'000;

    Int32 value = 0;

    Price() = default;

    Price& operator=(double v)
    {
        value = v * multiplier;
        return *this;
    }

    Price& operator=(std::string_view v)
    {
        int32_t price;
        auto    res = std::from_chars(v.begin(), v.end(), price);
        price *= multiplier;

        if (res.ptr != v.end() && *res.ptr == '.') {
            long        lv  = 0;
            const auto* beg = std::next(res.ptr);
            res             = std::from_chars(beg, v.end(), lv);
            price += (lv * multiplier) / std::pow(10, std::distance(beg, res.ptr));
        }
        value = price;
        return *this;
    }

    operator double() const
    {
        auto div = std::lldiv(value, multiplier);
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
// Timestamp. Number of nanoseconds since midnight
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
struct Timestamp
{
    UInt64 value = 0;

    Timestamp() = default;

    Timestamp(uint64_t v)
      : value(v)
    {
    }

    static Timestamp now()
    {
        // TODO
        return { static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
    }

    operator uint64_t() const
    {
        return static_cast<uint64_t>(value);
    }
};
#pragma pack()

inline std::string
to_string(const Timestamp& o)
{
    constexpr long secsinday = 60L * 60 * 24;
    long           days      = time(nullptr) / secsinday;
    return to_utcstring(std::chrono::nanoseconds(days * secsinday * 1000000000 + o.value), 9);
}

inline std::ostream&
operator<<(std::ostream& os, const Timestamp& o)
{
    return os << to_string(o);
}

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
      : value((std::underlying_type_t<LiquidityIndicator>(i) << 2) | (std::underlying_type_t<LiquidityInternalized>(n) << 4) |
              (std::underlying_type_t<LiquidityTopOfBook>(t) << 5) | (std::underlying_type_t<LiquiditySelfTrade>(s) << 6))
    {
    }

    explicit operator LiquidityIndicator() const
    {
        return LiquidityIndicator((value >> 2) & 0x03);
    }

    explicit operator LiquidityInternalized() const
    {
        return LiquidityInternalized((value >> 4) & 0x01);
    }

    explicit operator LiquidityTopOfBook() const
    {
        return LiquidityTopOfBook((value >> 5) & 0x01);
    }

    explicit operator LiquiditySelfTrade() const
    {
        return LiquiditySelfTrade((value >> 6) & 0x01);
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

#pragma pack(1)
struct RejectReason
{
    Int16 value = 0;

    RejectReason(int code = 0)
      : value(static_cast<int16_t>(code))
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

/*
enum class TimeInForce : uint8_t
{
        Day = 0,
        IOC = 3,
        FOK = 4,
};

inline std::string
to_json(TimeInForce o, bool verbose)
{
        // clang-format off
        if (verbose) {
                switch(o)
                {
                        case TimeInForce::Day: return to_json("0(Day)", verbose); break;
                        case TimeInForce::IOC: return to_json("3(IOC)", verbose); break;
                        case TimeInForce::FOK: return to_json("4(FOK)", verbose); break;
                        default: return to_json(std::to_string(static_cast<std::underlying_type_t<TimeInForce>>(o)) + "(Invalid TimeInForce)", verbose); break;
                };
        } else {
                switch(o)
                {
                        case TimeInForce::Day: return to_json(0, verbose); break;
                        case TimeInForce::IOC: return to_json(3, verbose); break;
                        case TimeInForce::FOK: return to_json(4, verbose); break;
                        default: return to_json(static_cast<std::underlying_type_t<TimeInForce>>(o), verbose); break;
                };
        }
        return to_json("", verbose);
        // clang-format on
}

inline std::string
to_string(TimeInForce o)
{
        switch (o) {
                        // clang-format off
                case TimeInForce::Day: return "0(Day)"; break;
                case TimeInForce::IOC: return "3(IOC)"; break;
                case TimeInForce::FOK: return "4(FOK)"; break;
                default: return std::to_string(static_cast<std::underlying_type_t<TimeInForce>>(o)) + "(Invalid TimeInForce)"; break;
                        // clang-format on
        };
}

inline std::ostream&
operator<<(std::ostream& s, TimeInForce o)
{
        return s << to_string(o);
}

enum class OpenClose : uint8_t
{
        Ignore = 0,
};

inline std::string
to_json(OpenClose o, bool verbose)
{
        // clang-format off
        if (verbose) {
                switch(o)
                {
                        case OpenClose::Ignore: return to_json("0(Ignore)", verbose); break;
                        default: return to_json(std::to_string(static_cast<std::underlying_type_t<OpenClose>>(o)) + "(Invalid OpenClose)", verbose); break;
                };
        } else {
                switch(o)
                {
                        case OpenClose::Ignore: return to_json(0, verbose); break;
                        default: return to_json(static_cast<std::underlying_type_t<OpenClose>>(o), verbose); break;
                };
        }
        return to_json("", verbose);
        // clang-format on
}

inline std::string
to_string(OpenClose o)
{
        switch (o) {
                        // clang-format off
                case OpenClose::Ignore: return "0(Ignore)"; break;
                default: return std::to_string(static_cast<std::underlying_type_t<OpenClose>>(o)) + "(Invalid OpenClose)"; break;
                        // clang-format on
        };
}

inline std::ostream&
operator<<(std::ostream& s, OpenClose o)
{
        return s << to_string(o);
}

enum class OrderState : uint8_t
{
        OnBook	  = 1,
        NotOnBook = 2,
};

inline std::string
to_json(OrderState o, bool verbose)
{
        // clang-format off
        if (verbose) {
                switch(o)
                {
                        case OrderState::OnBook:    return to_json("1(OnBook)"   , verbose); break;
                        case OrderState::NotOnBook: return to_json("2(NotOnBook)", verbose); break;
                        default: return to_json(std::to_string(static_cast<std::underlying_type_t<OrderState>>(o)) + "(Invalid OrderState)", verbose); break;
                };
        } else {
                switch(o)
                {
                        case OrderState::OnBook:    return to_json(1, verbose); break;
                        case OrderState::NotOnBook: return to_json(2, verbose); break;
                        default: return to_json(static_cast<std::underlying_type_t<OrderState>>(o), verbose); break;
                };
        }
        return to_json("", verbose);
        // clang-format on
}

inline std::string
to_string(OrderState o)
{
        switch (o) {
                        // clang-format off
                case OrderState::OnBook:    return "1(OnBook)";    break;
                case OrderState::NotOnBook: return "2(NotOnBook)"; break;
                default: return std::to_string(static_cast<std::underlying_type_t<OrderState>>(o)) + "(Invalid OrderState)"; break;
                        // clang-format on
        };
}

inline std::ostream&
operator<<(std::ostream& s, OrderState o)
{
        return s << to_string(o);
}
*/

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

} // namespace OUCH::OUCH50

#endif
