#ifndef Paxsim_Examples_Telnet_Filter_dot_h
#define Paxsim_Examples_Telnet_Filter_dot_h

#include "context.h"

#include "core/streamlog.h"
#include "core/types.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Filter. We decide what to do by looking at the incoming text.
//---------------------------------------------------------------------------------------------------------------------
class Filter : public tag::Control
{
public:
    Filter(Context& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Demonstrate message drop
        if (message.find("drop") != std::string::npos || message.find("Drop") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Dropping" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return {};
        }

        // Demonstrate break using control session. Finish message processing and then stop the session.
        if (m_context.action == Context::Action::Break) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Breaking" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { { message }, {} };
        }
        // Demonstrate abort using control session. Stop message processing and then stop the session.
        if (m_context.action == Context::Action::Abort) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Aborting" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { {} };
        }
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Continuing" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { { message } };
    }

    // Control interface
    std::vector<std::optional<std::string>> execute(Context::Action command)
    {
        if (command == Context::Action::Abort) {
            log << level::info << ctlmark << '[' << command << ']' << std::endl;
            return { {} };
        }
        return {};
    }

private:
    Context& m_context;
};

} // namespace text
#endif
