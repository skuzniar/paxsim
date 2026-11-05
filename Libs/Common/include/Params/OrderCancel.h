#ifndef Common_Params_OrderCancel_dot_h
#define Common_Params_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderCancel
{
    using Config = Common::Config;

public:
    explicit OrderCancel(const Config& config)
    {
        init(config);
    }

    std::map<int, Config> quantity;
    std::map<int, Config> leaves;
    std::map<int, Config> below;

private:
    auto load(const Config& config)
    {
        for (const auto& entry : config) {
            if (const auto quantity = static_cast<int>(entry["Quantity"]); quantity != 0) {
                this->quantity.emplace(quantity, entry);
            }
            if (const auto quantity = static_cast<int>(entry["Leaves"]); quantity != 0) {
                this->leaves.emplace(quantity, entry);
            }
            if (const auto quantity = static_cast<int>(entry["Below"]); quantity != 0) {
                this->below.emplace(quantity, entry);
            }
        }
        log << level::debug << here << " Loaded " << quantity.size() << " quantity entries." << std::endl;
        log << level::debug << here << " Loaded " << leaves.size() << " leaves entries." << std::endl;
        log << level::debug << here << " Loaded " << below.size() << " below entries." << std::endl;
    }

    void init(const Config& config)
    {
        load(config["Modules.OrderCancel"]);
    }
};

} // namespace Params
#endif
