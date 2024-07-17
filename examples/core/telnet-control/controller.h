#ifndef Paxsim_Examples_Telnet_Controller_dot_h
#define Paxsim_Examples_Telnet_Controller_dot_h

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
// Text protocol Controller. Used as both, the message processor to parse user commands and pass them to the context,
// and as the sourse of commands to be executed.
//---------------------------------------------------------------------------------------------------------------------
class Controller
{
public:
    Controller(Context& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << ctlmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Demonstrate break.
        if (message.find("break") != std::string::npos) {
            m_context.action = Context::Action::Break;
        }
        // Demonstrate abort.
        if (message.find("abort") != std::string::npos) {
            m_context.action = Context::Action::Abort;
        }
        // Reset
        if (message.find("reset") != std::string::npos) {
            m_context.action = Context::Action::Continue;
        }
        return {};
    }

    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

    std::vector<Context::Action> commands()
    {
        log << level::info << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << m_context.action << std::endl;
        if (m_context.action == Context::Action::None) {
            return {};
        }
        return { m_context.action };
    }

private:
    Context& m_context;
};

} // namespace text
#endif
