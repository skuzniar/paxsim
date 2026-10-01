#ifndef Common_Params_FillCancel_dot_h
#define Common_Params_FillCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace Common;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class FillCancel
{
public:
    explicit FillCancel(const Config& config)
    {
        init(config);
    }

    using Key = std::pair<unsigned, double>;
    std::map<Key, Config> cancel;

    auto load(const Config& config)
    {
        for (const auto& entry : config) {
            this->cancel.emplace(Key(static_cast<int>(entry["Quantity"]), static_cast<double>(entry["Price"])), entry);
        }
        log << level::debug << here << " Loaded " << cancel.size() << " cancel entries." << std::endl;
    }

    void init(const Config& config)
    {
        load(config["Modules.FillCancel"]);
    }
};

} // namespace Common::Params
#endif
