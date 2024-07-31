#ifndef Simulator_Fix42_Common_Factory_dot_h
#define Simulator_Fix42_Common_Factory_dot_h

#include <quickfix/Message.h>

#include <Core/Order.h>

namespace Fix42::Common {

using namespace Simulator::Core;

struct Factory
{
    static Order order(const FIX::Message& m)
    {
        return Order(m.getField(FIX::FIELD::ClOrdID),
                     m.getField(FIX::FIELD::Symbol),
                     from_string<Order::Side>(m.getField(FIX::FIELD::Side)),
                     from_string<Order::Type>(m.getField(FIX::FIELD::OrdType)),
                     atoi(m.getField(FIX::FIELD::OrderQty).c_str()),
                     atof(m.getField(FIX::FIELD::Price).c_str()));
    }

    static Order order(const Order& o, const FIX::Message& m)
    {
        return Order(m.getField(FIX::FIELD::ClOrdID),
                     m.isSetField(FIX::FIELD::Symbol) ? m.getField(FIX::FIELD::Symbol) : o.symbol(),
                     m.isSetField(FIX::FIELD::Side) ? from_string<Order::Side>(m.getField(FIX::FIELD::Side)) : o.side(),
                     m.isSetField(FIX::FIELD::OrdType) ? from_string<Order::Type>(m.getField(FIX::FIELD::OrdType))
                                                       : o.type(),
                     m.isSetField(FIX::FIELD::OrderQty) ? atoi(m.getField(FIX::FIELD::OrderQty).c_str()) : o.quantity(),
                     m.isSetField(FIX::FIELD::Price) ? atof(m.getField(FIX::FIELD::Price).c_str()) : o.price());
    }
};

} // namespace Fix42::Common
#endif
