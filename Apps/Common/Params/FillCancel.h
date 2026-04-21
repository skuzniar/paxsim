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
    std::map<Key, Config::Table> cancel;

    auto load(const Config& config, const char* path)
    {
        if (Config::Table table = config[path]; table.valid()) {
            for (int i = 1; i <= table.size(); ++i) {
                Config::Table entry = table[i];
                if (auto [i, b] = this->cancel.emplace(Key(entry["Quantity"], entry["Price"]), entry); b) {
                    log << level::debug << path << ':' << i->second << std::endl;
                }
            }
        }
    }

    void init(const Config& config)
    {
        load(config, "Modules.FillCancel");
    }
};

} // namespace Common::Params
#endif
