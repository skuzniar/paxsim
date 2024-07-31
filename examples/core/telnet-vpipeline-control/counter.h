#ifndef Paxsim_Examples_Telnet_Counter_dot_h
#define Paxsim_Examples_Telnet_Counter_dot_h

#include "paxsim/core/streamlog.h"
#include "paxsim/core/types.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Counter. We prepend incoming messages with a counter. And process controller commands
//---------------------------------------------------------------------------------------------------------------------
class Counter : public tag::Control
{
public:
    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << " Counting" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;

        return { { '[' + std::to_string(++m_counter) + ']' + " " + message } };
    }

    // Control interface
    std::vector<std::optional<std::string>> execute(int command)
    {
        if (command == 100) {
            log << level::info << ctlmark << '[' << command << ']' << std::endl;
            return { {} };
        }
        return {};
    }

private:
    size_t m_counter = 0;
};

} // namespace text
#endif
