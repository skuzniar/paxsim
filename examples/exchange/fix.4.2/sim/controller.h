#ifndef Paxsim_Examples_Fix42_Sim_Controller_dot_h
#define Paxsim_Examples_Fix42_Sim_Controller_dot_h

#include "core/streamlog.h"
#include "core/types.h"

#include <string>
#include <vector>
#include <optional>

#include <json5cpp.h>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Order Book Context. Provides storage for orders and control commands.
//---------------------------------------------------------------------------------------------------------------------
struct ControllerContext
{
    ControllerContext(const json& cfg)
    {
    }

    // Controll commands
    std::string post(const json& message)
    {
        log << level::info << vmark << '[' << message << ']' << std::endl;
        Queue = message;
        return "{}";
    }

    json Queue;
};

//---------------------------------------------------------------------------------------------------------------------
// Controller is a source of controller commands.
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
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

    std::vector<json> commands()
    {
        if (!m_context.Queue.empty()) {
            log << level::info << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << m_context.Queue << std::endl;
            std::vector<json> ret{ m_context.Queue };
            m_context.Queue.clear();
            return ret;
        }
        return {};
    }

private:
    ControllerContext& m_context;
};

} // namespace fix42::sim
#endif
