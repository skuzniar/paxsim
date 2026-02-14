#ifndef Context_ExecutionBook_Execution_dot_h
#define Context_ExecutionBook_Execution_dot_h

#include "Context/OrderBook/Order.h"

namespace Fix::Context {

//-----------------------------------------------------------------------------------------------------------------
// Order Execution. Keeps track of the quantity, price and the state of the execution.
//-----------------------------------------------------------------------------------------------------------------
class Execution
{
public:
    using ID      = std::string;
    using OrderID = Order::ID;

    // Create a new execution.
    Execution(const Order& order, unsigned quantity, double price)
      : m_execution_id(Order::anExecutionID())
      , m_order_id(order.orderID())
      , m_quantity(quantity)
      , m_price(price)
    {
    }

    // Create a replacement execution.
    Execution(Execution& execution, unsigned quantity, double price)
      : m_execution_id(Order::anExecutionID())
      , m_order_id(execution.orderID())
      , m_quantity(quantity)
      , m_price(price)
    {
        execution.m_reference_id = this->executionID();
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

    bool live() const
    {
        return m_live;
    }

    void cancel() const
    {
        m_live = false;
    }

    // Auto-generated using nvim-cppgen
    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, const Execution& o)
    {
        // clang-format off
        s << "[Execution]=";
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
    OrderID          m_order_id;
    mutable unsigned m_quantity = 0;
    mutable double   m_price    = 0;
    mutable bool     m_live     = true;
};

} // namespace Fix::Context

#endif
