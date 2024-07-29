#ifndef Paxsim_Examples_Fix42_Ctl_Controller_dot_h
#define Paxsim_Examples_Fix42_Ctl_Controller_dot_h

#include "core/streamlog.h"
#include <json5cpp.h>
#include <vector>
#include <optional>

namespace Fix42::Control {

using namespace paxsim::core;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// JSON protocol Controller. Receives parsed Json message and posts it into the associated context.
//---------------------------------------------------------------------------------------------------------------------
template<typename Context>
class Controller
{
public:
    Controller(Context& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<json>> process(const json& message)
    {
        auto response = m_context.post(message);
        if (response.empty()) {
            return {};
        }
        return { response };
    }

private:
    Context& m_context;
};

} // namespace Fix42::Control
#endif
