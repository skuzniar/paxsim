#ifndef Paxsim_Control_Session_Command_dot_h
#define Paxsim_Control_Session_Command_dot_h

#include <string>
#include <iostream>
#include <variant>

namespace paxsim::control::session {

//----------------------------------------------------------------------------------------------------------------------
// Session Selector. TODO - Not sure how we can address session components.
//----------------------------------------------------------------------------------------------------------------------
struct Selector
{
    Selector() = default;

    template<typename Def>
    explicit Selector(const Def& def)
    {
    }

    struct Params
    {};

    friend std::ostream& operator<<(std::ostream& s, const Params& o)
    {
        // clang-forFillmat off
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
// Stop action. Will cause session termination
//----------------------------------------------------------------------------------------------------------------------
struct Stop
{
    Stop() = default;

    template<typename Def>
    explicit Stop(const Def& def)
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

    friend std::ostream& operator<<(std::ostream& s, const Stop& o)
    {
        // clang-format off
        s << "Stop: ";
        s << '{' << o.params << '}';
        // clang-format on
        return s;
    }

    Params params;
};

//----------------------------------------------------------------------------------------------------------------------
// Session Command. Consists of a Selector and a union of all possible Actions. Add actions as needed.
//----------------------------------------------------------------------------------------------------------------------
struct Command
{
    // TODO - For testing.
    Command()
      : action(Stop())
    {
    }

    template<typename Def>
    Command(const Def& sel, const Def& act)
      : selector(sel)
    {
        if (const auto& def = act["Stop"]) {
            this->action = Stop(def);
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
    std::variant<Stop> action;
};

} // namespace paxsim::control::session

#endif
