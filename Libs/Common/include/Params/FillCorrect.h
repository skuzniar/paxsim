#ifndef Common_Params_FillCorrect_dot_h
#define Common_Params_FillCorrect_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "Generic.h"
#include <cstddef>

namespace Params {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fill Correct scenario parameters.
//---------------------------------------------------------------------------------------------------------------------
class FillCorrect : public Generic
{
public:
    explicit FillCorrect(const Config& config)
    {
        init(config);
    }

    void init(const Config& config)
    {
        if (auto [present, cfg] = config("Control.Fill.Correct"); present) {
            auto count = load(cfg);
            log << level::debug << here << " Loaded " << count << " Fill Correct entries." << std::endl;
        }
    }

    void clear()
    {
        auto count = Generic::clear();
        log << level::debug << here << " Cleared " << count << " Fill Correct entries." << std::endl;
    }
};

} // namespace Params
#endif
