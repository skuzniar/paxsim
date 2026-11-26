#ifndef Control_Admin_dot_h
#define Control_Admin_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace Control {

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
                process(command);
            }
        } catch (const std::exception& e) {
            next.put(e.what());
            return false;
        }
        next.put("OK");
        return false;
    }

private:
    void process(const Config& command)
    {
        // TODO
        if (auto [present, _] = command("Clear"); present) {
            m_params.clear();
        }
    }

    Params& m_params;
};

} // namespace Control
#endif
