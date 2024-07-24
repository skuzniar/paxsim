#ifndef Paxsim_Examples_Telnet_Filter_dot_h
#define Paxsim_Examples_Telnet_Filter_dot_h

#include "context.h"

#include "core/streamlog.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Filter. We decide what to do by looking at the incoming text.
//---------------------------------------------------------------------------------------------------------------------
class Filter
{
public:
    Filter(Context& context)
    {
    }

    static std::vector<std::optional<std::string>> process(const std::string& message)
    {
        // Demonstrate message drop
        if (message.find("drop") != std::string::npos || message.find("Drop") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Dropping" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return {};
        }

        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Demonstrate abort. We will immediately stop message processing and then stop the session.
        if (message.find("abort") != std::string::npos || message.find("Abort") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Aborting" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { {} };
        }
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Continuing" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { { message } };
    }
};

} // namespace text
#endif
