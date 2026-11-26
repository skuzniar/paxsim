#ifndef Common_Params_OrderCancel_dot_h
#define Common_Params_OrderCancel_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "Generic.h"
#include <cstddef>

namespace Common::Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order Cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class OrderCancel : public Generic
{
public:
    explicit OrderCancel(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Control.Order.Cancel"); present) {
            auto count = load(cfg);
            log << level::debug << here << " Loaded " << count << " Order Cancel entries." << std::endl;
        }
    }

    void clear()
    {
        auto count = Generic::clear();
        log << level::debug << here << " Cleared " << count << " Order Cancel entries." << std::endl;
    }
};

} // namespace Common::Params
#endif
