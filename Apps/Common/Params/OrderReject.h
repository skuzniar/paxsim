#ifndef Common_Params_OrderReject_dot_h
#define Common_Params_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order reject scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderReject
{
public:
    explicit OrderReject(const Config& config)
    {
        init(config);
    }

    std::map<int, Config> enter;
    std::map<int, Config> replace;
    std::map<int, Config> cancel;

private:
    static auto load(const Config& config)
    {
        std::map<int, Config> map;
        for (const auto& entry : config) {
            if (auto [i, b] = map.emplace(entry["Quantity"], entry); b) {
                log << level::debug << i->second << std::endl;
            }
        }
        return map;
    }

    void init(const Config& config)
    {
        enter   = load(config["Modules.OrderReject.Enter"]);
        replace = load(config["Modules.OrderReject.Replace"]);
        cancel  = load(config["Modules.OrderReject.Cancel"]);
    }
};

} // namespace Common::Params
#endif
