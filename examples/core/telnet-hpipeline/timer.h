#ifndef Paxsim_Examples_Telnet_Timer_dot_h
#define Paxsim_Examples_Telnet_Timer_dot_h

#include "paxsim/core/streamlog.h"
#include "paxsim/core/types.h"
#include <string>
#include <vector>
#include <optional>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Timer. Trigger abort if no message received for a set number of seconds
//---------------------------------------------------------------------------------------------------------------------
class Timer : public tag::Cyclical
{
public:
    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
        m_counter = 30;
        return {};
    }

    std::vector<std::optional<std::string>> timeout()
    {
        std::string message =
            m_counter == 0 ? "Timer expired waiting for message." : "Waiting " + std::to_string(m_counter) + "...";
        log << level::info << vmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;

        if (m_counter-- == 0) {
            return { { message }, {} };
        }
        return {};
    }

    static timepoint timeout_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

private:
    std::size_t m_counter = 10;
};

} // namespace text
#endif
