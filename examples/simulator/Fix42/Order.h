#ifndef Paxsim_Examples_Fix42_Order_dot_h
#define Paxsim_Examples_Fix42_Order_dot_h

#include "quickfix/fix42/NewOrderSingle.h"

#include "Types.h"

#include <chrono>
#include <iostream>

namespace Fix42 {

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

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, Type o)
    {
        switch (o) {
                // clang-format off
            case Type::Market:                s << "Market";                break;
            case Type::Limit:                 s << "Limit";                 break;
            case Type::Stop:                  s << "Stop";                  break;
            case Type::StopLimit:             s << "StopLimit";             break;
            case Type::MarketOnClose:         s << "MarketOnClose";         break;
            case Type::WithOrWithout:         s << "WithOrWithout";         break;
            case Type::LimitOrBetter:         s << "LimitOrBetter";         break;
            case Type::LimitWithOrWithout:    s << "LimitWithOrWithout";    break;
            case Type::OnBasis:               s << "OnBasis";               break;
            case Type::OnClose:               s << "OnClose";               break;
            case Type::LimitOnClose:          s << "LimitOnClose";          break;
            case Type::ForexMarket:           s << "ForexMarket";           break;
            case Type::PreviouslyQuoted:      s << "PreviouslyQuoted";      break;
            case Type::PreviouslyIndicated:   s << "PreviouslyIndicated";   break;
            case Type::ForexLimit:            s << "ForexLimit";            break;
            case Type::ForexSwap:             s << "ForexSwap";             break;
            case Type::ForexPreviouslyQuoted: s << "ForexPreviouslyQuoted"; break;
            case Type::Funari:                s << "Funari";                break;
            case Type::Pegged:                s << "Pegged";                break;
                // clang-format on
        };
        return s;
    }

    /*
    enum class HandlingInstructions : char
    {
        AutomatedPrivate = '1',
        AutomatedPublic  = '2',
        Manual           = '3'
    };

    enum class ExecutionTransactionType : char
    {
        New     = '0',
        Cancel  = '1',
        Correct = '2',
        Status  = '3'
    };
    */

    Order(const FIX42::NewOrderSingle& message);

    Order(const Order& order, const FIX42::NewOrderSingle& message);

    ID id() const
    {
        return clientOrderID();
    }

    ID clientOrderID() const
    {
        return m_client_order_id;
    }

    std::string exchangeOrderID() const
    {
        return 'O' + std::to_string(m_exchng_order_id);
    }

    static std::string executionID()
    {
        return 'E' + std::to_string(create_id());
    }

    Status status() const
    {
        return m_status;
    }

    const std::string& symbol() const
    {
        return m_symbol;
    }

    const Side side() const
    {
        return m_side;
    }

    const Type type() const
    {
        return m_type;
    }

    void setStatus(Status s)
    {
        m_status = s;
    }

    unsigned orderQuantity() const
    {
        return m_order_quantity;
    }

    double price() const
    {
        return m_price;
    }

    void setQuantity(int quantity)
    {
        m_order_quantity = quantity;
    }

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, const Order& o)
    {
        // clang-format off
        s << "[Order]=";
        s << "ExchngOrderId: " << o.m_exchng_order_id << ' ';
        s << "ClientOrderId: " << o.m_client_order_id << ' ';
        s << "Symbol: "        << o.m_symbol          << ' ';
        s << "Side: "          << o.m_side            << ' ';
        s << "Type: "          << o.m_type            << ' ';
        s << "Status: "        << o.m_status          << ' ';
        s << "OrderQuantity: " << o.m_order_quantity  << ' ';
        s << "Price: "         << o.m_price;
        // clang-format on
        return s;
    }

private:
    static uint64_t create_id()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

private:
    uint64_t    m_exchng_order_id = create_id();
    ID          m_client_order_id;
    std::string m_symbol;
    Side        m_side;
    Type        m_type;
    Status      m_status         = Status::New;
    unsigned    m_order_quantity = 0;
    double      m_price          = 0;
};

// Auto-generated using nvim-cppgen
template<>
Order::Status
from_string<Order::Status>(std::string_view v)
{
    if (v.size() != 1) {
        throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Order::Status.");
    }
    std::underlying_type_t<Order::Status> i = v[0];
    // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::New)             ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::PartiallyFilled) ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Filled)          ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::DoneForDay)      ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Canceled)        ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Replaced)        ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::PendingCancel)   ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Stopped)         ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Rejected)        ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Suspended)       ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::PendingNew)      ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Calculated)      ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Expired)         ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::Accepted)        ||
            i == static_cast<std::underlying_type_t<Order::Status>>(Order::Status::PendingReplace) ;
    // clang-format on
    if (!b) {
        throw std::runtime_error("Value " + std::to_string(i) + " is outside of Order::Status enumeration range.");
    }
    return static_cast<Order::Status>(i);
}

// Auto-generated using nvim-cppgen
template<>
Order::Side
from_string<Order::Side>(std::string_view v)
{
    if (v.size() != 1) {
        throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Order::Side.");
    }
    std::underlying_type_t<Order::Side> i = v[0];

    // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::Buy)             ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::Sell)            ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::BuyMinus)        ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::SellPlus)        ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::SellShort)       ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::SellShortExempt) ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::Undisclosed)     ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::Cross)           ||
            i == static_cast<std::underlying_type_t<Order::Side>>(Order::Side::CrossShort)     ;
    // clang-format on
    if (!b) {
        throw std::runtime_error("Value " + std::to_string(i) + " is outside of Order::Side enumeration range.");
    }
    return static_cast<Order::Side>(i);
}

// Auto-generated using nvim-cppgen
template<>
Order::Type
from_string<Order::Type>(std::string_view v)
{
    if (v.size() != 1) {
        throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Order::Type.");
    }
    std::underlying_type_t<Order::Type> i = v[0];
    // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::Market)                ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::Limit)                 ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::Stop)                  ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::StopLimit)             ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::MarketOnClose)         ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::WithOrWithout)         ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::LimitOrBetter)         ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::LimitWithOrWithout)    ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::OnBasis)               ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::OnClose)               ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::LimitOnClose)          ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::ForexMarket)           ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::PreviouslyQuoted)      ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::PreviouslyIndicated)   ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::ForexLimit)            ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::ForexSwap)             ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::ForexPreviouslyQuoted) ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::Funari)                ||
            i == static_cast<std::underlying_type_t<Order::Type>>(Order::Type::Pegged)               ;
    // clang-format on
    if (!b) {
        throw std::runtime_error("Value " + std::to_string(i) + " is outside of Order::Type enumeration range.");
    }
    return static_cast<Order::Type>(i);
}

// Moved outside of the class and after conversion specialization
Order::Order(const FIX42::NewOrderSingle& message)
  : m_client_order_id(message.getField(FIX::FIELD::ClOrdID))
  , m_symbol(message.getField(FIX::FIELD::Symbol))
  , m_side(from_string<Order::Side>(message.getField(FIX::FIELD::Side)))
  , m_type(from_string<Order::Type>(message.getField(FIX::FIELD::OrdType)))
  , m_order_quantity(atoi(message.getField(FIX::FIELD::OrderQty).c_str()))
  , m_price(atof(message.getField(FIX::FIELD::Price).c_str()))
{
}

Order::Order(const Order& order, const FIX42::NewOrderSingle& message)
  : m_client_order_id(message.getField(FIX::FIELD::ClOrdID))
  , m_symbol(message.isSetField(FIX::FIELD::Symbol) ? message.getField(FIX::FIELD::Symbol) : order.symbol())
  , m_side(message.isSetField(FIX::FIELD::Side) ? from_string<Order::Side>(message.getField(FIX::FIELD::Side))
                                                : order.side())
  , m_type(message.isSetField(FIX::FIELD::OrdType) ? from_string<Order::Type>(message.getField(FIX::FIELD::OrdType))
                                                   : order.type())
  , m_order_quantity(message.isSetField(FIX::FIELD::OrderQty) ? atoi(message.getField(FIX::FIELD::OrderQty).c_str())
                                                              : order.orderQuantity())
  , m_price(message.isSetField(FIX::FIELD::Price) ? atof(message.getField(FIX::FIELD::Price).c_str()) : order.price())
{
}

} // namespace Fix42

#endif
