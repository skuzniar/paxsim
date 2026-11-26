#ifndef Params_FillCancel_dot_h
#define Params_FillCancel_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "Generic.h"
#include <cstddef>

namespace Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill Cancel scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class FillCancel : public Generic
{
public:
    explicit FillCancel(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Control.Fill.Cancel"); present) {
            auto count = load(cfg);
            log << level::debug << here << " Loaded " << count << " Fill Cancel entries." << std::endl;
        }
    }

    void clear()
    {
        auto count = Generic::clear();
        log << level::debug << here << " Cleared " << count << " Fill Cancel entries." << std::endl;
    }
};

} // namespace Params
#endif
