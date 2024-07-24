#ifndef Paxsim_Examples_Telnet_Filter_dot_h
#define Paxsim_Examples_Telnet_Filter_dot_h

#include "core/streamlog.h"
#include "core/types.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Filter. We decide what to do by looking at the incoming text. And we process controller commands.
//---------------------------------------------------------------------------------------------------------------------
class Filter : public tag::Control
{
public:
    static std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Demonstrate message drop
        if (message.find("drop") != std::string::npos || message.find("Drop") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Dropping" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return {};
        }
        // Demonstrate break. We will finish message processing and then stop the session.
        if (message.find("break") != std::string::npos || message.find("Break") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Breaking" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { { message }, {} };
        }
        // Demonstrate abort. We will immediately stop message processing and then stop the session.
        if (message.find("abort") != std::string::npos || message.find("Abort") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Aborting" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { { message } };
        }
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Continuing" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { { message } };
    }

    // Control interface
    static std::vector<std::optional<std::string>> execute(int command)
    {
        if (command == 100) {
            log << level::info << ctlmark << '[' << command << ']' << std::endl;
            return { {} };
        }
        return {};
    }

};

} // namespace text
#endif
