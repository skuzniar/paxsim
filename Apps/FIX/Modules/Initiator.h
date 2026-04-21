#ifndef FIX_Modules_Initiator_dot_h
#define FIX_Modules_Initiator_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"

#include "Common/Config.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol session initiator. Triggers login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Initiator
{
public:
    using State = Context::Session::State;

    template<typename Context>
    Initiator(const Config& config, Context& context)
      : m_context(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    void init(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        if (m_context.state() == State::LogonWait) {
            next.put(m_factory.logon());
            m_context.state(State::LogonSent);
        }
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace FIX::Modules
#endif
