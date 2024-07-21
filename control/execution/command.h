#ifndef Paxsim_Control_Execution_Command_dot_h
#define Paxsim_Control_Execution_Command_dot_h

#include <string>
#include <iostream>

namespace paxsim::control::execution {

//----------------------------------------------------------------------------------------------------------------------
// Execution Selector. Used to identify executions which will be targeted by the action.
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
// Bust action. Applied to selected executions.
//----------------------------------------------------------------------------------------------------------------------
struct Bust
{
    Bust() = default;

    template<typename Def>
    explicit Bust(const Def& def)
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

    friend std::ostream& operator<<(std::ostream& s, const Bust& o)
    {
        // clang-format off
        s << "Bust: ";
        s << '{' << o.params << '}';
        // clang-format on
        return s;
    }

    Params params;
};

//----------------------------------------------------------------------------------------------------------------------
// Execution Command. Consists of a Selector and a union of all possible Actions. Add actions as needed.
//----------------------------------------------------------------------------------------------------------------------
struct Command
{
    template<typename Def>
    Command(const Def& sel, const Def& act)
      : selector(sel)
    {
        if (const auto& def = act["Bust"]) {
            this->action = Bust(def);
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

private:
    Selector           selector;
    std::variant<Bust> action;
};

} // namespace paxsim::control::execution

#endif
