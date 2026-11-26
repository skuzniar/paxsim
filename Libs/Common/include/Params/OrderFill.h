#ifndef Common_Params_OrderFill_dot_h
#define Common_Params_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "Generic.h"
#include <cstddef>

namespace Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order Fill scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderFill : public Generic
{
public:
    explicit OrderFill(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Control.Order.Fill"); present) {
            auto count = load(cfg);
            log << level::debug << here << " Loaded " << count << " Order Fill entries." << std::endl;
        }
    }

    void clear()
    {
        auto count = Generic::clear();
        log << level::debug << here << " Cleared " << count << " Order Fill entries." << std::endl;
    }
};

} // namespace Params
#endif
