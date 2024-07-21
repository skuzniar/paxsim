#ifndef Paxsim_Control_Command_dot_h
#define Paxsim_Control_Command_dot_h

#include "order/command.h"
#include "execution/command.h"

#include <string>
#include <iostream>

namespace paxsim::control {

//----------------------------------------------------------------------------------------------------------------------
// Control Command operates on a specific part of the simulator. Control Command Target designates this part. Here we
// name these parts. Add more targets as needed.
//----------------------------------------------------------------------------------------------------------------------
enum class Target
{
    Order,
    Execution
};

// clang-format off
template<typename T> inline T from_string(const std::string& v);
// clang-format on

// Auto-generated using nvim-cppgen
template<>
inline Target
from_string(const std::string& v)
{
    // clang-format off
    if (v == "Order")     { return Target::Order; }
    if (v == "Execution") { return Target::Execution; }
    throw std::runtime_error("Value " + v + " is outside of Target enumeration range.");
    // clang-format on
}

//----------------------------------------------------------------------------------------------------------------------
// Generic Control Command. It is a collection module commands. Add more commands as needed.
//----------------------------------------------------------------------------------------------------------------------
template<typename... Cmds>
struct Commands : public std::variant<Cmds...>
{
    using base = std::variant<Cmds...>;

    template<typename Cmd>
    Commands(Cmd&& cmd)
      : base(std::forward<Cmd>(cmd))
    {
    }

    friend std::ostream& operator<<(std::ostream& s, const Commands& o)
    {
        std::visit([&](const auto& x) { s << x; }, o);
        return s;
    }
};

using Command = Commands<order::Command, execution::Command>;

//----------------------------------------------------------------------------------------------------------------------
// Command Factory creates commands. Here we impose a requirements on the definition type.
//----------------------------------------------------------------------------------------------------------------------
struct Factory
{
    template<typename Def>
    static Command command(const Def& def)
    {
        std::string target = def["Target"].template as<std::string>();
        if (target == "Order") {
            return order::Command(def["Selector"], def["Action"]);
        }
        if (target == "Execution") {
            return execution::Command(def["Selector"], def["Action"]);
        }
        throw std::runtime_error("Unknown target: " + target);
    }
};

} // namespace paxsim::control

#endif
