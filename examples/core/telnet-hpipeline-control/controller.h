#ifndef Paxsim_Examples_Telnet_Controller_dot_h
#define Paxsim_Examples_Telnet_Controller_dot_h

#include "paxsim/core/streamlog.h"
#include "paxsim/core/types.h"

#include <vector>

namespace text {
using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Controller. Used as a source of controller commands.
//---------------------------------------------------------------------------------------------------------------------
class Controller
{
public:
    static timepoint control_at()
    {
        constexpr int delay = 30;
        log << level::info << ctlmark << '[' << "Will send control command in " << delay << " seconds." << ']' << std::endl;
        return std::chrono::steady_clock::now() + std::chrono::seconds(delay);
    }

    std::vector<int> commands()
    {
        return { 100 };
    }
};

} // namespace text
#endif
