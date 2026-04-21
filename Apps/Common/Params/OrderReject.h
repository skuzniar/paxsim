#ifndef Common_Params_OrderReject_dot_h
#define Common_Params_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace Common;
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

    std::map<int, Config::Table> enter;
    std::map<int, Config::Table> replace;
    std::map<int, Config::Table> cancel;

private:
    static auto load(const Config& config, const char* path)
    {
        std::map<int, Config::Table> map;
        if (Config::Table table = config[path]; table.valid()) {
            for (int i = 1; i <= table.size(); ++i) {
                Config::Table entry = table[i];
                if (auto [i, b] = map.emplace(entry["Quantity"], entry); b) {
                    log << level::debug << path << ':' << i->second << std::endl;
                }
            }
        }
        return map;
    }

    void init(const Config& config)
    {
        enter   = load(config, "Modules.OrderReject.Enter");
        replace = load(config, "Modules.OrderReject.Replace");
        cancel  = load(config, "Modules.OrderReject.Cancel");
    }
};

} // namespace Common::Params
#endif
