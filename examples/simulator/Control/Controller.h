#ifndef Simulator_Control_Controller_dot_h
#define Simulator_Control_Controller_dot_h

#include <paxsim/core/streamlog.h>
#include <paxsim/core/types.h>

#include <paxsim/control/command.h>

#include <json5cpp.h>
#include <vector>
#include <optional>
#include <queue>

namespace Simulator::Control {

using namespace paxsim::core;
using namespace paxsim::control;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// Simulator Controller Context. Provides connection between Controller and Simulator sessions. Controller session posts
// messages into it and the Simulator session pulls commands from it.
//---------------------------------------------------------------------------------------------------------------------
struct ControllerContext
{
    template<typename Cfg>
    ControllerContext(const Cfg& cfg)
    {
    }

    // Controll commands
    std::string post(const json& message)
    {
        // log << level::info << vmark << '[' << message << ']' << std::endl;
        try {
            commands.push(Factory::command(message));
        } catch (const std::exception& e) {
            auto error = std::string("Error converting controller message into command. Failure: ") + e.what() + '.';
            log << level::error << error << std::endl;
            return error;
        }
        return {};
    }

    std::queue<Command> commands;
};

//---------------------------------------------------------------------------------------------------------------------
// Simulator Controller module. When used as a handler module in the Controller session, it receives parsed Json
// messages and posts them into the associated context. When used as a controller module on the Simulator session, it
// pulls commands from the context.
//---------------------------------------------------------------------------------------------------------------------
class Controller
{
public:
    Controller(ControllerContext& context)
      : m_context(context)
    {
    }

    // Controller session
    std::vector<std::optional<json>> process(const json& message)
    {
        auto response = m_context.post(message);
        if (response.empty()) {
            return {};
        }
        return { response };
    }

    // Simulator session
    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
    }

    std::vector<Command> commands()
    {
        std::vector<Command> result;
        while (!m_context.commands.empty() and result.size() < 100) {
            result.emplace_back(m_context.commands.front());
            m_context.commands.pop();
        }
        return result;
    }

private:
    ControllerContext& m_context;
};

} // namespace Simulator::Control
#endif
