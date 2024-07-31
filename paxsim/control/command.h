#ifndef Paxsim_Control_Command_dot_h
#define Paxsim_Control_Command_dot_h

#include <paxsim/control/session/command.h>
#include <paxsim/control/order/command.h>
#include <paxsim/control/execution/command.h>

#include <string>
#include <iostream>
#include <variant>

namespace paxsim::control {

//----------------------------------------------------------------------------------------------------------------------
// Control Commands operate on a specific part of the simulator. Control Command Target designates this part. Here we
// name these parts. Add more targets as needed.
//----------------------------------------------------------------------------------------------------------------------
enum class Target
{
    Session,
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
    if (v == "Session")   { return Target::Session; }
    if (v == "Order")     { return Target::Order; }
    if (v == "Execution") { return Target::Execution; }
    throw std::runtime_error("Value " + v + " is outside of Target enumeration range.");
    // clang-format on
}

//----------------------------------------------------------------------------------------------------------------------
// Generic Control Command. It is a collection of module commands. Add more commands as needed.
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

using Command = Commands<session::Command, order::Command, execution::Command>;

//----------------------------------------------------------------------------------------------------------------------
// Command Factory. Here we create all known module commands.
//----------------------------------------------------------------------------------------------------------------------
struct Factory
{
    template<typename Def>
    static Command command(const Def& def)
    {
        std::string target = def["Target"].template as<std::string>();
        if (target == "Session") {
            return session::Command(def["Selector"], def["Action"]);
        }
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
