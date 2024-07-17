#ifndef Paxsim_Examples_Fix42_Ctl_Controller_dot_h
#define Paxsim_Examples_Fix42_Ctl_Controller_dot_h

#include "core/streamlog.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <optional>

namespace fix42::ctl {

using namespace paxsim::core;

using paxsim::core::log;

using json = nlohmann::json;

//---------------------------------------------------------------------------------------------------------------------
// JSON protocol Controller. Posts the control message to the associated context.
//---------------------------------------------------------------------------------------------------------------------
template<typename Context>
class Controller
{
    static constexpr const char* iam = "Controller";

public:
    Controller(Context& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<json>> process(const json& message)
    {
        // log << level::info << ctlmark << iam << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
        auto response = m_context.post(message);
        if (response.empty()) {
            return {};
        }
        return { response };
    }

private:
    Context& m_context;
};

} // namespace fix42::ctl
#endif
