#ifndef Context_OrderBook_Order_dot_h
#define Context_OrderBook_Order_dot_h

#include <string>
#include <ostream>
#include <cstddef>

namespace Fix::Context {

//-----------------------------------------------------------------------------------------------------------------
// Exchange order. Keeps track of order state.
//-----------------------------------------------------------------------------------------------------------------
class Order
{
public:
    using ID = std::string;

    enum class Status : char
    {
        New             = '0',
        PartiallyFilled = '1',
        Filled          = '2',
        DoneForDay      = '3',
        Canceled        = '4',
        Replaced        = '5',
        PendingCancel   = '6',
        Stopped         = '7',
        Rejected        = '8',
        Suspended       = '9',
        PendingNew      = 'A',
        Calculated      = 'B',
        Expired         = 'C',
        Accepted        = 'D',
        PendingReplace  = 'E'
    };

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, Status o)
    {
        switch (o) {
                // clang-format off
            case Status::New:             s << "New";             break;
            case Status::PartiallyFilled: s << "PartiallyFilled"; break;
            case Status::Filled:          s << "Filled";          break;
            case Status::DoneForDay:      s << "DoneForDay";      break;
            case Status::Canceled:        s << "Canceled";        break;
            case Status::Replaced:        s << "Replaced";        break;
            case Status::PendingCancel:   s << "PendingCancel";   break;
            case Status::Stopped:         s << "Stopped";         break;
            case Status::Rejected:        s << "Rejected";        break;
            case Status::Suspended:       s << "Suspended";       break;
            case Status::PendingNew:      s << "PendingNew";      break;
            case Status::Calculated:      s << "Calculated";      break;
            case Status::Expired:         s << "Expired";         break;
            case Status::Accepted:        s << "Accepted";        break;
            case Status::PendingReplace:  s << "PendingReplace";  break;
                // clang-format on
        };
        return s;
    }

    template<typename T>
    static [[cppgen::auto]] T enum_cast(std::string_view v);
    template<typename T>
    static [[cppgen::auto]] T enum_cast(int v);

    template<>
    [[cppgen::auto]] Status enum_cast<Status>(std::string_view v)
    {
        // clang-format off
        if (v == "0") return Status::New;
        if (v == "1") return Status::PartiallyFilled;
        if (v == "2") return Status::Filled;
        if (v == "3") return Status::DoneForDay;
        if (v == "4") return Status::Canceled;
        if (v == "5") return Status::Replaced;
        if (v == "6") return Status::PendingCancel;
        if (v == "7") return Status::Stopped;
        if (v == "8") return Status::Rejected;
        if (v == "9") return Status::Suspended;
        if (v == "A") return Status::PendingNew;
        if (v == "B") return Status::Calculated;
        if (v == "C") return Status::Expired;
        if (v == "D") return Status::Accepted;
        if (v == "E") return Status::PendingReplace;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of Status enumeration range.");
    }
    template<>
    [[cppgen::auto]] Status enum_cast<Status>(int v)
    {
        if (
            // clang-format off
            v == static_cast<std::underlying_type_t<Status>>(Status::New)             ||
            v == static_cast<std::underlying_type_t<Status>>(Status::PartiallyFilled) ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Filled)          ||
            v == static_cast<std::underlying_type_t<Status>>(Status::DoneForDay)      ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Canceled)        ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Replaced)        ||
            v == static_cast<std::underlying_type_t<Status>>(Status::PendingCancel)   ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Stopped)         ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Rejected)        ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Suspended)       ||
            v == static_cast<std::underlying_type_t<Status>>(Status::PendingNew)      ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Calculated)      ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Expired)         ||
            v == static_cast<std::underlying_type_t<Status>>(Status::Accepted)        ||
            v == static_cast<std::underlying_type_t<Status>>(Status::PendingReplace) )
        // clang-format on
        {
            return static_cast<Status>(v);
        }
        throw std::out_of_range("Value " + std::to_string(v) + " is outside of Status enumeration range.");
    }

    enum class Side : char
    {
        Buy             = '1',
        Sell            = '2',
        BuyMinus        = '3',
        SellPlus        = '4',
        SellShort       = '5',
        SellShortExempt = '6',
        Undisclosed     = '7',
        Cross           = '8',
        CrossShort      = '9'
    };

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, Side o)
    {
        switch (o) {
                // clang-format off
            case Side::Buy:             s << "Buy";             break;
            case Side::Sell:            s << "Sell";            break;
            case Side::BuyMinus:        s << "BuyMinus";        break;
            case Side::SellPlus:        s << "SellPlus";        break;
            case Side::SellShort:       s << "SellShort";       break;
            case Side::SellShortExempt: s << "SellShortExempt"; break;
            case Side::Undisclosed:     s << "Undisclosed";     break;
            case Side::Cross:           s << "Cross";           break;
            case Side::CrossShort:      s << "CrossShort";      break;
                // clang-format on
        };
        return s;
    }

    template<>
    [[cppgen::auto]] Side enum_cast<Side>(std::string_view v)
    {
        // clang-format off
        if (v == "1") return Side::Buy;
        if (v == "2") return Side::Sell;
        if (v == "3") return Side::BuyMinus;
        if (v == "4") return Side::SellPlus;
        if (v == "5") return Side::SellShort;
        if (v == "6") return Side::SellShortExempt;
        if (v == "7") return Side::Undisclosed;
        if (v == "8") return Side::Cross;
        if (v == "9") return Side::CrossShort;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of Side enumeration range.");
    }
    template<>
    [[cppgen::auto]] Side enum_cast<Side>(int v)
    {
        if (
            // clang-format off
            v == static_cast<std::underlying_type_t<Side>>(Side::Buy)             ||
            v == static_cast<std::underlying_type_t<Side>>(Side::Sell)            ||
            v == static_cast<std::underlying_type_t<Side>>(Side::BuyMinus)        ||
            v == static_cast<std::underlying_type_t<Side>>(Side::SellPlus)        ||
            v == static_cast<std::underlying_type_t<Side>>(Side::SellShort)       ||
            v == static_cast<std::underlying_type_t<Side>>(Side::SellShortExempt) ||
            v == static_cast<std::underlying_type_t<Side>>(Side::Undisclosed)     ||
            v == static_cast<std::underlying_type_t<Side>>(Side::Cross)           ||
            v == static_cast<std::underlying_type_t<Side>>(Side::CrossShort)     )
        // clang-format on
        {
            return static_cast<Side>(v);
        }
        throw std::out_of_range("Value " + std::to_string(v) + " is outside of Side enumeration range.");
    }

    enum class Type : char
    {
        Market                = '1',
        Limit                 = '2',
        Stop                  = '3',
        StopLimit             = '4',
        MarketOnClose         = '5',
        WithOrWithout         = '6',
        LimitOrBetter         = '7',
        LimitWithOrWithout    = '8',
        OnBasis               = '9',
        OnClose               = 'A',
        LimitOnClose          = 'B',
        ForexMarket           = 'C',
        PreviouslyQuoted      = 'D',
        PreviouslyIndicated   = 'E',
        ForexLimit            = 'F',
        ForexSwap             = 'G',
        ForexPreviouslyQuoted = 'H',
        Funari                = 'I',
        Pegged                = 'P'
    };

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, Type o)
    {
        switch (o) {
                // clang-format off
            case Type::Market:                s << "'1'(Market)";                break;
            case Type::Limit:                 s << "'2'(Limit)";                 break;
            case Type::Stop:                  s << "'3'(Stop)";                  break;
            case Type::StopLimit:             s << "'4'(StopLimit)";             break;
            case Type::MarketOnClose:         s << "'5'(MarketOnClose)";         break;
            case Type::WithOrWithout:         s << "'6'(WithOrWithout)";         break;
            case Type::LimitOrBetter:         s << "'7'(LimitOrBetter)";         break;
            case Type::LimitWithOrWithout:    s << "'8'(LimitWithOrWithout)";    break;
            case Type::OnBasis:               s << "'9'(OnBasis)";               break;
            case Type::OnClose:               s << "'A'(OnClose)";               break;
            case Type::LimitOnClose:          s << "'B'(LimitOnClose)";          break;
            case Type::ForexMarket:           s << "'C'(ForexMarket)";           break;
            case Type::PreviouslyQuoted:      s << "'D'(PreviouslyQuoted)";      break;
            case Type::PreviouslyIndicated:   s << "'E'(PreviouslyIndicated)";   break;
            case Type::ForexLimit:            s << "'F'(ForexLimit)";            break;
            case Type::ForexSwap:             s << "'G'(ForexSwap)";             break;
            case Type::ForexPreviouslyQuoted: s << "'H'(ForexPreviouslyQuoted)"; break;
            case Type::Funari:                s << "'I'(Funari)";                break;
            case Type::Pegged:                s << "'P'(Pegged)";                break;
            default: s << std::to_string(static_cast<std::underlying_type_t<Type>>(o)) + "(Invalid Type)"; break;
                // clang-format on
        };
        return s;
    }

    template<>
    [[cppgen::auto]] Type enum_cast<Type>(std::string_view v)
    {
        // clang-format off
        if (v == "1") return Type::Market;
        if (v == "2") return Type::Limit;
        if (v == "3") return Type::Stop;
        if (v == "4") return Type::StopLimit;
        if (v == "5") return Type::MarketOnClose;
        if (v == "6") return Type::WithOrWithout;
        if (v == "7") return Type::LimitOrBetter;
        if (v == "8") return Type::LimitWithOrWithout;
        if (v == "9") return Type::OnBasis;
        if (v == "A") return Type::OnClose;
        if (v == "B") return Type::LimitOnClose;
        if (v == "C") return Type::ForexMarket;
        if (v == "D") return Type::PreviouslyQuoted;
        if (v == "E") return Type::PreviouslyIndicated;
        if (v == "F") return Type::ForexLimit;
        if (v == "G") return Type::ForexSwap;
        if (v == "H") return Type::ForexPreviouslyQuoted;
        if (v == "I") return Type::Funari;
        if (v == "P") return Type::Pegged;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of Type enumeration range.");
    }
    template<>
    [[cppgen::auto]] Type enum_cast<Type>(int v)
    {
        if (
            // clang-format off
            v == static_cast<std::underlying_type_t<Type>>(Type::Market)                ||
            v == static_cast<std::underlying_type_t<Type>>(Type::Limit)                 ||
            v == static_cast<std::underlying_type_t<Type>>(Type::Stop)                  ||
            v == static_cast<std::underlying_type_t<Type>>(Type::StopLimit)             ||
            v == static_cast<std::underlying_type_t<Type>>(Type::MarketOnClose)         ||
            v == static_cast<std::underlying_type_t<Type>>(Type::WithOrWithout)         ||
            v == static_cast<std::underlying_type_t<Type>>(Type::LimitOrBetter)         ||
            v == static_cast<std::underlying_type_t<Type>>(Type::LimitWithOrWithout)    ||
            v == static_cast<std::underlying_type_t<Type>>(Type::OnBasis)               ||
            v == static_cast<std::underlying_type_t<Type>>(Type::OnClose)               ||
            v == static_cast<std::underlying_type_t<Type>>(Type::LimitOnClose)          ||
            v == static_cast<std::underlying_type_t<Type>>(Type::ForexMarket)           ||
            v == static_cast<std::underlying_type_t<Type>>(Type::PreviouslyQuoted)      ||
            v == static_cast<std::underlying_type_t<Type>>(Type::PreviouslyIndicated)   ||
            v == static_cast<std::underlying_type_t<Type>>(Type::ForexLimit)            ||
            v == static_cast<std::underlying_type_t<Type>>(Type::ForexSwap)             ||
            v == static_cast<std::underlying_type_t<Type>>(Type::ForexPreviouslyQuoted) ||
            v == static_cast<std::underlying_type_t<Type>>(Type::Funari)                ||
            v == static_cast<std::underlying_type_t<Type>>(Type::Pegged)               )
        // clang-format on
        {
            return static_cast<Type>(v);
        }
        throw std::out_of_range("Value " + std::to_string(v) + " is outside of Type enumeration range.");
    }

    enum class HandlingInstructions : char
    {
        AutomatedPrivate = '1',
        AutomatedPublic  = '2',
        Manual           = '3'
    };

    template<>
    [[cppgen::auto]] HandlingInstructions enum_cast<HandlingInstructions>(std::string_view v)
    {
        // clang-format off
        if (v == "1") return HandlingInstructions::AutomatedPrivate;
        if (v == "2") return HandlingInstructions::AutomatedPublic;
        if (v == "3") return HandlingInstructions::Manual;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of HandlingInstructions enumeration range.");
    }
    template<>
    [[cppgen::auto]] HandlingInstructions enum_cast<HandlingInstructions>(int v)
    {
        if (
            // clang-format off
            v == static_cast<std::underlying_type_t<HandlingInstructions>>(HandlingInstructions::AutomatedPrivate) ||
            v == static_cast<std::underlying_type_t<HandlingInstructions>>(HandlingInstructions::AutomatedPublic)  ||
            v == static_cast<std::underlying_type_t<HandlingInstructions>>(HandlingInstructions::Manual)          )
        // clang-format on
        {
            return static_cast<HandlingInstructions>(v);
        }
        throw std::out_of_range("Value " + std::to_string(v) + " is outside of HandlingInstructions enumeration range.");
    }

    enum class ExecutionTransactionType : char
    {
        New     = '0',
        Cancel  = '1',
        Correct = '2',
        Status  = '3'
    };

    template<>
    [[cppgen::auto]] ExecutionTransactionType enum_cast<ExecutionTransactionType>(std::string_view v)
    {
        // clang-format off
        if (v == "0") return ExecutionTransactionType::New;
        if (v == "1") return ExecutionTransactionType::Cancel;
        if (v == "2") return ExecutionTransactionType::Correct;
        if (v == "3") return ExecutionTransactionType::Status;
        // clang-format on
        throw std::out_of_range("Value " + std::string(v) + " is outside of ExecutionTransactionType enumeration range.");
    }
    template<>
    [[cppgen::auto]] ExecutionTransactionType enum_cast<ExecutionTransactionType>(int v)
    {
        if (
            // clang-format off
            v == static_cast<std::underlying_type_t<ExecutionTransactionType>>(ExecutionTransactionType::New)     ||
            v == static_cast<std::underlying_type_t<ExecutionTransactionType>>(ExecutionTransactionType::Cancel)  ||
            v == static_cast<std::underlying_type_t<ExecutionTransactionType>>(ExecutionTransactionType::Correct) ||
            v == static_cast<std::underlying_type_t<ExecutionTransactionType>>(ExecutionTransactionType::Status) )
        // clang-format on
        {
            return static_cast<ExecutionTransactionType>(v);
        }
        throw std::out_of_range("Value " + std::to_string(v) + " is outside of ExecutionTransactionType enumeration range.");
    }

    Order() = default;

    Order(const std::string& client_order_id, const std::string& symbol, Side side, Type type, unsigned quantity, double price)
      : m_client_order_id(client_order_id)
      , m_symbol(symbol)
      , m_side(side)
      , m_type(type)
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    std::string orderID() const
    {
        return m_order_id;
    }

    std::string chainID() const
    {
        return m_chain_id;
    }

    static std::string anOrderID()
    {
        return 'O' + std::to_string(create_id());
    }

    std::string clientOrderID() const
    {
        return m_client_order_id;
    }

    static std::string anExecutionID()
    {
        return 'E' + std::to_string(create_id());
    }

    Status status() const
    {
        return m_status;
    }

    void status(Status s) const
    {
        m_status = s;
    }

    const std::string& symbol() const
    {
        return m_symbol;
    }

    Side side() const
    {
        return m_side;
    }

    Type type() const
    {
        return m_type;
    }

    void setStatus(Status s) const
    {
        m_status = s;
    }

    unsigned quantity() const
    {
        return m_quantity;
    }

    double price() const
    {
        return m_price;
    }

    void setQuantity(int quantity)
    {
        m_quantity = quantity;
    }

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, const Order& o)
    {
        // clang-format off
        s << "[Order]=";
        s << "OrderId: "       << o.m_order_id        << ' ';
        s << "ChainId: "       << o.m_chain_id        << ' ';
        s << "ClientOrderId: " << o.m_client_order_id << ' ';
        s << "Symbol: "        << o.m_symbol          << ' ';
        s << "Side: "          << o.m_side            << ' ';
        s << "Type: "          << o.m_type            << ' ';
        s << "Status: "        << o.m_status          << ' ';
        s << "Quantity: "      << o.m_quantity        << ' ';
        s << "Price: "         << o.m_price;
        // clang-format on
        return s;
    }

private:
    static uint64_t create_id()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

private:
    std::string m_order_id = 'O' + std::to_string(create_id());
    std::string m_chain_id = m_order_id;

    std::string    m_client_order_id;
    std::string    m_symbol;
    Side           m_side;
    Type           m_type;
    mutable Status m_status   = Status::New;
    unsigned       m_quantity = 0;
    double         m_price    = 0;
};

} // namespace Fix::Context
#endif
