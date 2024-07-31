#ifndef Paxsim_Examples_Telnet_Breaker_dot_h
#define Paxsim_Examples_Telnet_Breaker_dot_h

#include "paxsim/core/streamlog.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Breaker. We decide to break processing when certain message is present
//---------------------------------------------------------------------------------------------------------------------
class Breaker
{
public:
    static std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Demonstrate break. We will finish message processing and then stop the session.
        if (message.find("break") != std::string::npos || message.find("Break") != std::string::npos) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Breaking" << ' ' << '['
                << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { {} };
        }
        return {};
    }
};

} // namespace text
#endif
