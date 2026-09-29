#ifndef Common_Params_FillCorrect_dot_h
#define Common_Params_FillCorrect_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace Common;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill correct scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class FillCorrect
{
public:
    explicit FillCorrect(const Config& config)
    {
        init(config);
    }

    using Key = std::pair<unsigned, double>;
    std::map<Key, Config> correct;

    auto load(const Config& config)
    {
        for (const auto& entry : config) {
            this->correct.emplace(Key(static_cast<int>(entry["Quantity"]), static_cast<double>(entry["Price"])), entry);
            log << level::debug << entry << std::endl;
        }
    }

    void init(const Config& config)
    {
        load(config["Modules.FillCorrect"]);
    }
};

} // namespace Common::Params
#endif
