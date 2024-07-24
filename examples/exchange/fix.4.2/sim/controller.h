#ifndef Paxsim_Examples_Fix42_Sim_Controller_dot_h
#define Paxsim_Examples_Fix42_Sim_Controller_dot_h

#include "core/streamlog.h"
#include "core/types.h"

#include "control/command.h"

#include <string>
#include <vector>
#include <optional>

#include <json5cpp.h>

namespace fix42::sim {

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
    static constexpr const char* iam = "Controller";

public:
    Controller(ControllerContext& context)
      : m_context(context)
    {
    }

    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

    // std::vector<paxsim::control::Command> commands()
    std::vector<paxsim::control::Command> commands()
    {
        if (m_context.command) {
            try {
                auto command = paxsim::control::Factory::command(m_context.command.value());
                m_context.command.reset();
                return std::vector{ command };
            } catch (const std::exception& e) {
                log << level::error << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << m_context.command.value()
                    << std::endl;
                log << "Error converting Json into command. Failure: " << e.what() << '.' << '\n';
            }
        }
        return {};
    }

private:
    ControllerContext& m_context;
};

} // namespace fix42::sim
#endif
