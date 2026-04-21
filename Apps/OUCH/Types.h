#ifndef OUCH_Types_dot_h
#define OUCH_Types_dot_h

#include "OUCH/OUCH50//Types.h"

#include <ostream>
#include <iostream>

namespace OUCH {

//-----------------------------------------------------------------------------------------------------------------
// Exchange order. Keeps track of order state.
//-----------------------------------------------------------------------------------------------------------------
class Order
{
public:
    using ID = uint32_t;

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

    Order(uint32_t clord_id, OUCH50::BuySellIndicator side, uint32_t book_id, unsigned quantity, double price)
      : m_clord_id(clord_id)
      , m_side(side)
      , m_book_id(book_id)
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    auto orderID() const
    {
        return m_order_id;
    }

    static auto anOrderID()
    {
        return create_id();
    }

    auto clordID() const
    {
        return m_clord_id;
    }

    static auto anExecutionID()
    {
        return create_id();
    }

    Status status() const
    {
        return m_status;
    }

    void status(Status s) const
    {
        m_status = s;
    }

    auto side() const
    {
        return m_side;
    }

    auto bookID() const
    {
        return m_book_id;
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

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, const Order& o)
    {
        // clang-format off
        s << "[Order]=";
        s << "OrderId: "  << o.m_order_id << ' ';
        s << "ClOrdId: "  << o.m_clord_id << ' ';
        s << "Side: "     << o.m_side     << ' ';
        s << "Type: "     << o.m_type     << ' ';
        s << "Status: "   << o.m_status   << ' ';
        s << "Quantity: " << o.m_quantity << ' ';
        s << "Price: "    << o.m_price;
        // clang-format on
        return s;
    }

private:
    static uint32_t create_id()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

private:
    using Side = OUCH50::BuySellIndicator;

    ID             m_order_id = create_id();
    ID             m_clord_id;
    Side           m_side;
    uint32_t       m_book_id;
    Type           m_type;
    mutable Status m_status   = Status::New;
    unsigned       m_quantity = 0;
    double         m_price    = 0;
};

//-----------------------------------------------------------------------------------------------------------------
// Order Fill. Keeps track of the quantity, price and the state of the fill.
//-----------------------------------------------------------------------------------------------------------------
class Fill
{
public:
    using ID = Order::ID;

    // Create a new fill.
    Fill(const Order& order, unsigned quantity, double price)
      : m_execution_id(Order::anExecutionID())
      , m_order_id(order.orderID())
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    // Create a replacement fill.
    Fill(Fill& fill, unsigned quantity, double price)
      : m_execution_id(Order::anExecutionID())
      , m_order_id(fill.m_order_id)
      , m_quantity(quantity)
      , m_price(price)
    {
        fill.m_reference_id = this->executionID();
    }

    auto id() const
    {
        return m_execution_id;
    }

    auto orderID() const
    {
        return m_order_id;
    }

    ID executionID() const
    {
        return m_execution_id;
    }

    ID referenceID() const
    {
        return m_reference_id;
    }

    unsigned quantity() const
    {
        return m_quantity;
    }

    double price() const
    {
        return m_price;
    }

    bool live() const
    {
        return m_live;
    }

    void cancel() const
    {
        m_live = false;
    }

    // Auto-generated using nvim-cppgen
    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, const Fill& o)
    {
        // clang-format off
        s << "[Fill]=";
        s << "ExecutionId: " << o.m_execution_id << ' ';
        s << "ReferenceId: " << o.m_reference_id << ' ';
        s << "OrderId: "     << o.m_order_id     << ' ';
        s << "Quantity: "    << o.m_quantity     << ' ';
        s << "Price: "       << o.m_price        << ' ';
        s << "Live: "        << o.m_live;
        // clang-format on
        return s;
    }

    // private:
    ID               m_execution_id;
    ID               m_reference_id;
    ID               m_order_id;
    mutable unsigned m_quantity = 0;
    mutable double   m_price    = 0;
    mutable bool     m_live     = true;
};

} // namespace OUCH

#endif
