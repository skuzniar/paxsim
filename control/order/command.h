#ifndef Paxsim_Control_Order_Command_dot_h
#define Paxsim_Control_Order_Command_dot_h

#include <string>
#include <iostream>
#include <variant>

namespace paxsim::control::order {

//----------------------------------------------------------------------------------------------------------------------
// Order Selector. Used to identify orders which will be targeted by the action.
//----------------------------------------------------------------------------------------------------------------------
struct Selector
{
    template<typename Def>
    explicit Selector(const Def& def)
    {
        if (const auto& val = def["Id"]) {
            params.id = val.template as<std::string>();
        }
        if (const auto& val = def["Quantity"]) {
            params.quantity = val.template as<int>();
        }
        if (const auto& val = def["Price"]) {
            params.price = val.template as<double>();
        }
    }

    struct Params
    {
        std::string id;
        int         quantity = 0;
        double      price    = 0.0;
    };

    friend std::ostream& operator<<(std::ostream& s, const Params& o)
    {
        // clang-forFillmat off
        s << "Id: " << o.id << ' ';
        s << "Quantity: " << o.quantity << ' ';
        s << "Price: " << o.price;
        // clang-format on
        return s;
    }

    friend std::ostream& operator<<(std::ostream& s, const Selector& o)
    {
        // clang-format off
        s << "Selector: ";
        s << '{' << o.params << '}';
        // clang-format on
        return s;
    }

    Params params;
};

//----------------------------------------------------------------------------------------------------------------------
// Fill action. Applied to selected orders.
//----------------------------------------------------------------------------------------------------------------------
struct Fill
{
    Fill() = default;

    template<typename Def>
    explicit Fill(const Def& def)
    {
        if (const auto& val = def["Quantity"]) {
            params.quantity = val.template as<int>();
        }
        if (const auto& val = def["Price"]) {
            params.price = val.template as<double>();
        }
    }

    struct Params
    {
        int    quantity = 0;
        double price    = 0.0;
    };

    friend std::ostream& operator<<(std::ostream& s, const Params& o)
    {
        // clang-format off
        s << "Quantity: " << o.quantity << ' ';
        s << "Price: "    << o.price;
        // clang-format on
        return s;
    }

    friend std::ostream& operator<<(std::ostream& s, const Fill& o)
    {
        // clang-format off
        s << "Fill: ";
        s << '{' << o.params << '}';
        // clang-format on
        return s;
    }

    Params params;
};

//----------------------------------------------------------------------------------------------------------------------
// Cancel action. Applied to selected orders.
//----------------------------------------------------------------------------------------------------------------------
struct Cancel
{
    Cancel() = default;

    template<typename Def>
    explicit Cancel(const Def& def)
    {
        if (const auto& val = def["Text"]) {
            params.text = val.template as<std::string>();
        }
    }

    struct Params
    {
        std::string text;
    };

    friend std::ostream& operator<<(std::ostream& s, const Params& o)
    {
        // clang-format off
        s << "Text: " << o.text;
        // clang-format on
        return s;
    }

    friend std::ostream& operator<<(std::ostream& s, const Cancel& o)
    {
        // clang-format off
        s << "Cancel: ";
        s << '{' << o.params << '}';
        // clang-format on
        return s;
    }

    Params params;
};

//----------------------------------------------------------------------------------------------------------------------
// Order Command. Consists of a Selector and a union of all possible Actions. Add actions as needed.
//----------------------------------------------------------------------------------------------------------------------
struct Command
{
    template<typename Def>
    Command(const Def& sel, const Def& act)
      : selector(sel)
    {
        if (const auto& def = act["Fill"]) {
            this->action = Fill(def);
        } else if (const auto& def = act["Cancel"]) {
            this->action = Cancel(def);
        } else {
            throw std::runtime_error("Unknown action.");
        }
    }

    friend std::ostream& operator<<(std::ostream& s, const Command& o)
    {
        // clang-format off
        s << o.selector << ' ';
        s << "Action: " << '{';
        std::visit([&](const auto &x) { s << x; }, o.action);
        s << '}';
        // clang-format on
        return s;
    }

    Selector                   selector;
    std::variant<Fill, Cancel> action;
};

} // namespace paxsim::control::order

#endif
