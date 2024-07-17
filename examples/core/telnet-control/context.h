#ifndef Paxsim_Examples_Telnet_Context_dot_h
#define Paxsim_Examples_Telnet_Context_dot_h

#include <cstddef>
#include <iostream>

namespace text {

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Context.
//---------------------------------------------------------------------------------------------------------------------
struct Context
{
    // Counter for messages
    std::size_t counter = 0;

    enum class Action
    {
        None,
        Continue,
        Break,
        Abort
    };

    // Auto-generated using nvim-cppgen
    friend std::ostream& operator<<(std::ostream& s, Action o)
    {
        switch(o)
        {
            // clang-format off
            case Action::None:     s << "None";     break;
            case Action::Continue: s << "Continue"; break;
            case Action::Break:    s << "Break";    break;
            case Action::Abort:    s << "Abort";    break;
            // clang-format on
        };
        return s;
    }

    // Termination flag set by the controller
    Action action = Action::Continue;
};

} // namespace text
#endif
