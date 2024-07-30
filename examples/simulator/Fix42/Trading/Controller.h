#ifndef Paxsim_Examples_Fix42_Sim_Controller_dot_h
#define Paxsim_Examples_Fix42_Sim_Controller_dot_h

#include "core/streamlog.h"
#include "core/types.h"

#include "control/command.h"

#include <string>
#include <vector>
#include <queue>

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
        //log << level::info << vmark << '[' << message << ']' << std::endl;
        commands.push(message);
        return {};
    }

    std::queue<json> commands;
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
        std::vector<Command> result;
        while (!m_context.commands.empty() and result.size() < 100) {
            try {
                result.emplace_back(Factory::command(m_context.commands.front()));
                m_context.commands.pop();
            } catch (const std::exception& e) {
                m_context.commands.pop();
                log << level::error << "Error converting Json into command. Failure: " << e.what() << '.' << std::endl;
            }
        }
        return result;
    }

private:
    ControllerContext& m_context;
};

} // namespace Fix42::Trading
#endif
