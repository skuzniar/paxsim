#ifndef OUCH_Modules_Feeder_dot_h
#define OUCH_Modules_Feeder_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include "Common/Config.h"

#include "OUCH/Context/Session.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Feeder
{
public:
    using State = Context::Session::State;

    template<typename Context>
    Feeder(const Config& config, Context& context)
      : m_session(context)
      , m_factory(context)
    {
    }

    // Periodically send message to the server
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_expr && m_session.state() == State::Normal) {
            feed(next);
        }
        return m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    }

private:
    template<typename Next>
    void feed(Next& next)
    {
        next.put(m_factory.enterOrder());
        next.put(m_factory.replaceOrder());
        next.put(m_factory.cancelOrder());

        next.put(m_factory.enterReject());
        next.put(m_factory.orderFill());
    }

    Context::Session& m_session;
    Factory           m_factory;

    timepoint m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(1);
};

} // namespace OUCH::Modules
#endif
