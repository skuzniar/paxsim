#ifndef Paxsim_Examples_Telnet_Counter_dot_h
#define Paxsim_Examples_Telnet_Counter_dot_h

#include "core/streamlog.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Counter. We prepend incoming messages with a counter.
//---------------------------------------------------------------------------------------------------------------------
class Counter
{
public:
    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        log << level::debug << _file_ << ':' << _line_ << ' ' << __func__ << " Counting" << ' ' << '['
            << hexdump(message.data(), message.size()) << ']' << std::endl;

        return { { '[' + std::to_string(++m_counter) + ']' + " " + message } };
    }

private:
    size_t m_counter = 0;
};

} // namespace text
#endif
