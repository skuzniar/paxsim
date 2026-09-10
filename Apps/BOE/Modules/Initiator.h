#ifndef BOE_Modules_Initiator_dot_h
#define BOE_Modules_Initiator_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "BOE/Context/Session.h"
#include "Common/Config.h"

namespace BOE::Modules {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// BOE protocol session initiator. Triggers login sequence.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Initiator
{
public:
    using Config = Common::Config;
    using State  = Context::Session::State;

    template<typename Context>
    explicit Initiator(Context& context)
      : m_context(context)
      , m_factory(context)
    {
    }

    template<typename Context>
    explicit Initiator(const Config&, Context& context)
      : m_context(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    void init(Next& next)
    {
        log << level::trace << ts << here << std::endl;
        if (m_context.state() == State::LogonWait) {
            next.put(m_factory.loginRequest());
            m_context.state(State::LogonSent);
        }
    }

private:
    Context::Session& m_context;
    Factory           m_factory;
};

} // namespace BOE::Modules
#endif
