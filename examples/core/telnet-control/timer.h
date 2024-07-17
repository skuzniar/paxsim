#ifndef Paxsim_Examples_Telnet_Timer_dot_h
#define Paxsim_Examples_Telnet_Timer_dot_h

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
// Text protocol Timer. Trigger abort if no message received for a set number of seconds.
//---------------------------------------------------------------------------------------------------------------------
class Timer : public tag::Cyclical
{
public:
    Timer(Context& context)
    {
    }

    std::vector<std::optional<std::string>> process(const std::string& message)
    {
        m_counter = 3;
        return {};
    }

    std::vector<std::optional<std::string>> timeout()
    {
        if (m_counter-- == 0) {
            std::string message = "Timer expired waiting for message.";
            log << level::info << hmark << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
            return { { message }, {} };
        }
        return {};
    }

    static timepoint timeout_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(10);
    }

private:
    std::size_t m_counter = 3;
};

} // namespace text
#endif
