#ifndef Paxsim_Examples_Telnet_Format_dot_h
#define Paxsim_Examples_Telnet_Format_dot_h

#include "core/streamlog.h"
#include <string>
#include <vector>
#include <optional>

namespace text {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol formatter.
//---------------------------------------------------------------------------------------------------------------------
class Format
{
public:
    std::vector<std::optional<std::string>> process(std::string message) const
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        // Remove leading spaces
        message = message.substr(message.find_first_not_of(" \t\n"));

        // Replace remaining new lines with spaces
        std::replace(message.begin(), message.end(), '\n', ' ');

        // Upcase first letter
        *message.begin() = std::toupper(*message.begin());

        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { { message } };
    }
};

} // namespace text
#endif
