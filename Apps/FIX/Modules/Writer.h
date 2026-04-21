#ifndef FIX_Modules_Writer_dot_h
#define FIX_Modules_Writer_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include "Common/Context/Session.h"
#include "Common/Config.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol output handler. Writes outgoing messages and generates heartbeat messages when needed.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Writer
{
public:
    template<typename Context>
    Writer(ORWBuffer& obuf, const Config& config, Context& context)
      : m_obuf(obuf)
      , m_context(context)
      , m_factory(context)
    {
    }

    template<typename Message>
    bool put(const Message& msg)
    {
        std::memcpy(m_obuf.wpos(), msg.data(), msg.size());
        m_obuf.wmove(msg.size());
        log << level::info << out << '[' << msg << ']' << std::endl;
        m_latest = std::chrono::steady_clock::now();
        return false;
    }

    // Send heartbeat message if no other message was sent for the heartbeat interval
    timepoint timeout(timepoint now)
    {
        if (m_context.state() == Context::Session::State::Normal) {
            if (now >= m_latest + std::chrono::seconds(m_context.HBInterval)) {
                this->put(m_factory.heartbeat());
            }
        }
        return m_latest + std::chrono::seconds(m_context.HBInterval);
    }

private:
    ORWBuffer&        m_obuf;
    Context::Session& m_context;
    Factory           m_factory;

    timepoint m_latest = std::chrono::steady_clock::now();
};

} // namespace FIX::Modules
#endif
