#ifndef Common_Control_Admin_dot_h
#define Common_Control_Admin_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace Common::Control {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Control Admin command processor.
//---------------------------------------------------------------------------------------------------------------------
template<typename Params>
class Admin
{
    using Config = Common::Config;

public:
    template<typename Context>
    explicit Admin(Context& context)
      : m_params(context)
    {
    }

    template<typename Context>
    Admin(const Config&, Context& context)
      : m_params(context)
    {
    }

    template<typename Next>
    bool put(const char* msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        try {
            Config config(msg);
            if (auto [present, command] = config("Control.Admin.Command"); present) {
                process(command, next);
                return false;
            }
        } catch (const std::exception& e) {
            next.put(e.what());
            return false;
        }
        return true;
    }

private:
    template<typename Next>
    void process(const Config& command, Next& next)
    {
        if (auto [present, _] = command("Clear"); present) {
            m_params.clear();
            next.put("OK");
        }
    }

    Params& m_params;
};

} // namespace Common::Control
#endif
