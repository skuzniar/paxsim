#ifndef Paxsim_Examples_Fix42_Sim_Controller_dot_h
#define Paxsim_Examples_Fix42_Sim_Controller_dot_h

#include "core/streamlog.h"
#include "core/types.h"

#include "control/command.h"

#include <string>
#include <vector>
#include <optional>

#include <json5cpp.h>

namespace Fix42::Trading {

using namespace paxsim::core;
using namespace paxsim::control;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// Controller Context. Provides connection between Controller and Simulator sessions.
//---------------------------------------------------------------------------------------------------------------------
struct ControllerContext
{
    ControllerContext(const json& cfg)
    {
    }

    // Controll commands
    std::string post(const json& message)
    {
        // log << level::info << vmark << '[' << message << ']' << std::endl;
        command = message;
        return {};
    }

    std::optional<json> command;
};

//---------------------------------------------------------------------------------------------------------------------
// Controller is a source of control commands. Controller attempts to convert Json objects into valid control commands.
//---------------------------------------------------------------------------------------------------------------------
class Controller
{
public:
    Controller(ControllerContext& context)
      : m_context(context)
    {
    }

    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
    }

    std::vector<Command> commands()
    {
        if (m_context.command) {
            try {
                auto command = Factory::command(m_context.command.value());
                m_context.command.reset();
                return std::vector{ command };
            } catch (const std::exception& e) {
                m_context.command.reset();
                log << level::error << "Error converting Json into command. Failure: " << e.what() << '.' << std::endl;
            }
        }
        return {};
    }

private:
    ControllerContext& m_context;
};

} // namespace Fix42::Trading
#endif
