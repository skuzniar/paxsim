#ifndef OUCH_Modules_Initiator_dot_h
#define OUCH_Modules_Initiator_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/Session.h"
#include "Common/Config.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH protocol session initiator. Triggers login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Initiator
{
public:
    using State = Context::Session::State;

    template<typename Context>
    explicit Initiator(const Config& config, Context& context)
      : m_context(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    void init(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        if (m_context.state() == State::LogonWait) {
            next.put(m_factory.loginRequest(m_context.UserName, m_context.Password));
            m_context.state(State::LogonSent);
        }
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace OUCH::Modules
#endif
