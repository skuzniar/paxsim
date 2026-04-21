#ifndef Common_Params_OrderCancel_dot_h
#define Common_Params_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include <map>
#include <cstddef>

namespace Common::Params {

using namespace Common;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderCancel
{
public:
    explicit OrderCancel(const Config& config)
    {
        init(config);
    }

    std::map<int, Config::Table> quantity;
    std::map<int, Config::Table> leaves;
    std::map<int, Config::Table> below;

private:
    auto load(const Config& config, const char* path)
    {
        if (Config::Table table = config[path]; table.valid()) {
            for (int i = 1; i <= table.size(); ++i) {
                Config::Table entry = table[i];
                if (const auto quantity = entry["Quantity"]; quantity.valid()) {
                    if (auto [i, b] = this->quantity.emplace(quantity, entry); b) {
                        log << level::debug << path << ':' << i->second << std::endl;
                    }
                }
                if (const auto quantity = entry["Leaves"]; quantity.valid()) {
                    if (auto [i, b] = this->leaves.emplace(quantity, entry); b) {
                        log << level::debug << path << ':' << i->second << std::endl;
                    }
                }
                if (const auto quantity = entry["Below"]; quantity.valid()) {
                    if (auto [i, b] = this->below.emplace(quantity, entry); b) {
                        log << level::debug << path << ':' << i->second << std::endl;
                    }
                }
            }
        }
    }

    void init(const Config& config)
    {
        load(config, "Modules.OrderCancel");
    }
};

} // namespace Common::Params
#endif
