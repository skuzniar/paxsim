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

    using Fills = std::vector<Config::Table>;
    std::map<int, Fills> fills;

private:
    auto load(const Config& config, const char* path)
    {
        std::map<int, std::vector<Config::Table>> fills;
        if (Config::Table table = config[path]; table.valid()) {
            for (int i = 1; i <= table.size(); ++i) {
                Config::Table entry    = table[i];
                int           quantity = entry["Quantity"];
                if (Config::Table table = entry["Fills"]; table.valid()) {
                    for (int i = 1; i <= table.size(); ++i) {
                        Config::Table entry = table[i];
                        this->fills[quantity].emplace_back(entry);
                    }
                }
                log << level::debug << path << ' ' << entry << std::endl;
            }
        }
    }

    void init(const Config& config)
    {
        load(config, "Modules.OrderFill");
    }
};

} // namespace Common::Params
#endif
