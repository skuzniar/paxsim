#ifndef Paxsim_Examples_Telnet_Handler_dot_h
#define Paxsim_Examples_Telnet_Handler_dot_h

#include "core/streamlog.h"
#include "core/types.h"
#include <string>
#include <vector>
#include <optional>

namespace text {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Handler. We decide what to do by looking at the incoming text.
//---------------------------------------------------------------------------------------------------------------------
class Handler
  : public tag::Cyclical
  , public tag::Control
{
public:
    std::vector<std::optional<std::string>> process(std::string message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
        m_counter = 30;

        // Format the incoming message by removing leading spaces, replacing new lines with spaces an upcasing first
        // letter
        message = message.substr(message.find_first_not_of(" \t\n"));

        std::replace(message.begin(), message.end(), '\n', ' ');

        *message.begin() = std::toupper(*message.begin());

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
            return { {} };
        }
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Continuing" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { { message } };
    }

    // Cyclical interface
    std::vector<std::optional<std::string>> timeout()
    {
        std::string message =
            m_counter == 0 ? "Timer expired waiting for message." : "Waiting " + std::to_string(m_counter) + "...";
        log << level::trace << ts << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        if (m_counter-- == 0) {
            return { { message }, {} };
        }
        return {};
    }

    static timepoint timeout_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

    // Control interface
    std::vector<std::optional<std::string>> execute(int command)
    {
        if (command == 100) {
            log << level::trace << ts << ctlmark << '[' << command << ']' << std::endl;
            return { {} };
        }
        return {};
    }

private:
    std::size_t m_counter = 10;
};

} // namespace text
#endif
