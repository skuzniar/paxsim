#ifndef Common_Control_Loader_dot_h
#define Common_Control_Loader_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace Common::Control {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Control parameters loader. Parses control command and applies it to control parameters.
//---------------------------------------------------------------------------------------------------------------------
template<typename Params>
class Loader
{
    using Config = Common::Config;

public:
    template<typename Context>
    explicit Loader(Context& context)
      : m_params(context)
    {
    }

    template<typename Context>
    Loader(const Config&, Context& context)
      : m_params(context)
    {
    }

    template<typename Next>
    bool put(const char* msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        try {
            m_params.init(Config(msg));
        } catch (const std::exception& e) {
            next.put(e.what());
            return true;
        }
        next.put("OK");
        return true;
    }

private:
    Params& m_params;
};

} // namespace Common::Control
#endif
