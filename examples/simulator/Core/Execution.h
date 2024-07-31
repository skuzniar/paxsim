#ifndef Simulator_Core_Execution_dot_h
#define Simulator_Core_Execution_dot_h

#include "Order.h"

#include <iostream>

namespace Simulator::Core {

//-----------------------------------------------------------------------------------------------------------------
// Order Execution. Keeps track of the quantity, price and the state of the execution.
//-----------------------------------------------------------------------------------------------------------------
class Execution
{
public:
    using ID      = std::string;
    using OrderID = Order::ID;

    Execution(const Order& order, unsigned quantity, double price)
      : m_execution_id(Order::executionID())
      , m_order_id(order.id())
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    Execution(const Execution& execution, unsigned quantity, double price)
      : m_execution_id(Order::executionID())
      , m_reference_id(execution.id())
      , m_order_id(execution.orderID())
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    ID id() const
    {
        return m_execution_id;
    }

    OrderID orderID() const
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

    bool active() const
    {
        return m_reference_id.empty();
    }

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, const Execution& o)
    {
        // clang-format off
        s << "[Execution]=";
        s << "ExecutionId: " << o.m_execution_id << ' ';
        s << "ReferenceId: " << o.m_reference_id << ' ';
        s << "OrderId: "     << o.m_order_id     << ' ';
        s << "Quantity: "    << o.m_quantity     << ' ';
        s << "Price: "       << o.m_price;
        // clang-format on
        return s;
    }

    // private:
    ID       m_execution_id;
    ID       m_reference_id;
    OrderID  m_order_id;
    unsigned m_quantity = 0;
    double   m_price    = 0;
};

} // namespace Simulator::Core

#endif
