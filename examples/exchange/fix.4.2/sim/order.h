#ifndef Paxsim_Examples_Fix42_Sim_Order_dot_h
#define Paxsim_Examples_Fix42_Sim_Order_dot_h

#include "quickfix/fix42/NewOrderSingle.h"

#include <chrono>
#include <iostream>

namespace fix42::sim {

//-----------------------------------------------------------------------------------------------------------------
// Exchange order. Keeps track of order state and executions.
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

    template<typename T>
    T from_string(std::string_view v);

    // Auto-generated using nvim-cppgen
    template<>
    Status from_string<Status>(std::string_view v)
    {
        if (v.size() != 1) {
            throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Side.");
        }
        std::underlying_type_t<Type> i = v[0];
        // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Status>>(Status::New)             ||
            i == static_cast<std::underlying_type_t<Status>>(Status::PartiallyFilled) ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Filled)          ||
            i == static_cast<std::underlying_type_t<Status>>(Status::DoneForDay)      ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Canceled)        ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Replaced)        ||
            i == static_cast<std::underlying_type_t<Status>>(Status::PendingCancel)   ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Stopped)         ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Rejected)        ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Suspended)       ||
            i == static_cast<std::underlying_type_t<Status>>(Status::PendingNew)      ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Calculated)      ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Expired)         ||
            i == static_cast<std::underlying_type_t<Status>>(Status::Accepted)        ||
            i == static_cast<std::underlying_type_t<Status>>(Status::PendingReplace) ;
        // clang-format on
        if (!b) {
            throw std::runtime_error("Value " + std::to_string(i) + " is outside of Status enumeration range.");
        }
        return static_cast<Status>(i);
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
    template<>
    Side from_string<Side>(std::string_view v)
    {
        if (v.size() != 1) {
            throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Side.");
        }
        std::underlying_type_t<Type> i = v[0];

        // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Side>>(Side::Buy)             ||
            i == static_cast<std::underlying_type_t<Side>>(Side::Sell)            ||
            i == static_cast<std::underlying_type_t<Side>>(Side::BuyMinus)        ||
            i == static_cast<std::underlying_type_t<Side>>(Side::SellPlus)        ||
            i == static_cast<std::underlying_type_t<Side>>(Side::SellShort)       ||
            i == static_cast<std::underlying_type_t<Side>>(Side::SellShortExempt) ||
            i == static_cast<std::underlying_type_t<Side>>(Side::Undisclosed)     ||
            i == static_cast<std::underlying_type_t<Side>>(Side::Cross)           ||
            i == static_cast<std::underlying_type_t<Side>>(Side::CrossShort)     ;
        // clang-format on
        if (!b) {
            throw std::runtime_error("Value " + std::to_string(i) + " is outside of Side enumeration range.");
        }
        return static_cast<Side>(i);
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
    template<>
    Type from_string<Type>(std::string_view v)
    {
        if (v.size() != 1) {
            throw std::runtime_error("Unable to convert " + std::string(v) + " into an underlying type of Type.");
        }
        std::underlying_type_t<Type> i = v[0];
        // clang-format off
        bool b =
            i == static_cast<std::underlying_type_t<Type>>(Type::Market)                ||
            i == static_cast<std::underlying_type_t<Type>>(Type::Limit)                 ||
            i == static_cast<std::underlying_type_t<Type>>(Type::Stop)                  ||
            i == static_cast<std::underlying_type_t<Type>>(Type::StopLimit)             ||
            i == static_cast<std::underlying_type_t<Type>>(Type::MarketOnClose)         ||
            i == static_cast<std::underlying_type_t<Type>>(Type::WithOrWithout)         ||
            i == static_cast<std::underlying_type_t<Type>>(Type::LimitOrBetter)         ||
            i == static_cast<std::underlying_type_t<Type>>(Type::LimitWithOrWithout)    ||
            i == static_cast<std::underlying_type_t<Type>>(Type::OnBasis)               ||
            i == static_cast<std::underlying_type_t<Type>>(Type::OnClose)               ||
            i == static_cast<std::underlying_type_t<Type>>(Type::LimitOnClose)          ||
            i == static_cast<std::underlying_type_t<Type>>(Type::ForexMarket)           ||
            i == static_cast<std::underlying_type_t<Type>>(Type::PreviouslyQuoted)      ||
            i == static_cast<std::underlying_type_t<Type>>(Type::PreviouslyIndicated)   ||
            i == static_cast<std::underlying_type_t<Type>>(Type::ForexLimit)            ||
            i == static_cast<std::underlying_type_t<Type>>(Type::ForexSwap)             ||
            i == static_cast<std::underlying_type_t<Type>>(Type::ForexPreviouslyQuoted) ||
            i == static_cast<std::underlying_type_t<Type>>(Type::Funari)                ||
            i == static_cast<std::underlying_type_t<Type>>(Type::Pegged)               ;
        // clang-format on
        if (!b) {
            throw std::runtime_error("Value " + std::to_string(i) + " is outside of Type enumeration range.");
        }
        return static_cast<Type>(i);
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

    Order(const FIX42::NewOrderSingle& message)
      : m_client_order_id(message.getField(FIX::FIELD::ClOrdID))
      , m_symbol(message.getField(FIX::FIELD::Symbol))
      , m_side(from_string<Side>(message.getField(FIX::FIELD::Side)))
      , m_type(from_string<Type>(message.getField(FIX::FIELD::OrdType)))
      , m_order_quantity(atoi(message.getField(FIX::FIELD::OrderQty).c_str()))
      , m_price(atof(message.getField(FIX::FIELD::Price).c_str()))
    {
    }

    Order(const Order& order, const FIX42::NewOrderSingle& message)
      : m_client_order_id(message.getField(FIX::FIELD::ClOrdID))
      , m_symbol(message.isSetField(FIX::FIELD::Symbol) ? message.getField(FIX::FIELD::Symbol) : order.symbol())
      , m_side(message.isSetField(FIX::FIELD::Side) ? from_string<Side>(message.getField(FIX::FIELD::Side))
                                                    : order.side())
      , m_type(message.isSetField(FIX::FIELD::OrdType) ? from_string<Type>(message.getField(FIX::FIELD::OrdType))
                                                       : order.type())
      , m_order_quantity(message.isSetField(FIX::FIELD::OrderQty) ? atoi(message.getField(FIX::FIELD::OrderQty).c_str())
                                                                  : order.orderQuantity())
      , m_price(message.isSetField(FIX::FIELD::Price) ? atof(message.getField(FIX::FIELD::Price).c_str())
                                                      : order.price())
    {
    }

    ID clientOrderID() const
    {
        return m_client_order_id;
    }

    std::string exchangeOrderID() const
    {
        return 'O' + std::to_string(m_exchorder_id);
    }

    std::string executionID()
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

    unsigned fillsQuantity() const
    {
        return m_fills_quantity;
    }

    unsigned leavesQuantity() const
    {
        return m_order_quantity - m_fills_quantity;
    }

    double avgPrice() const
    {
        return m_avgprice;
    }

    double price() const
    {
        return m_price;
    }

    virtual void setQuantity(int quantity)
    {
        m_order_quantity = quantity < m_fills_quantity ? m_fills_quantity : quantity;
    }

    virtual void execute(int quantity, double price)
    {
        assert(m_cumquantity + quantity <= m_ordquantity);
        m_avgprice = (m_avgprice * m_fills_quantity + price * quantity) / (m_fills_quantity + quantity);
        m_order_quantity -= quantity;
        m_fills_quantity += quantity;

        if (m_fills_quantity > 0) {
            m_status = m_fills_quantity < m_order_quantity ? Status::PartiallyFilled : Status::Filled;
        }
    }

private:
    static uint64_t create_id()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

private:
    uint64_t    m_exchorder_id = create_id();
    ID          m_client_order_id;
    std::string m_symbol;
    Side        m_side;
    Type        m_type;
    Status      m_status         = Status::New;
    unsigned    m_order_quantity = 0;
    unsigned    m_fills_quantity = 0;
    double      m_price          = 0;
    double      m_avgprice       = 0;
};

} // namespace fix42::sim

#endif
