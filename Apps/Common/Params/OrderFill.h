#ifndef Common_Params_OrderFill_dot_h
#define Common_Params_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace Common;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order fill scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderFill
{
public:
    explicit OrderFill(const Config& config)
    {
        init(config);
    }

    using Fills = std::vector<Config>;
    std::map<int, Fills> fills;

private:
    void load(const Config& config)
    {
        for (const auto& entry : config) {
            int quantity = static_cast<int>(entry["Quantity"]);
            for (const auto& fill : entry["Fills"]) {
                this->fills[quantity].emplace_back(fill);
            }
        }
        log << level::debug << here << " Loaded " << fills.size() << " fills entries." << std::endl;
    }
    void init(const Config& config)
    {
        load(config["Modules.OrderFill"]);
    }
};

} // namespace Common::Params
#endif
