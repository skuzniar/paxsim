#ifndef Modules_Writer_Writer_dot_h
#define Modules_Writer_Writer_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include <quickfix/Message.h>

#include "Context/Session/Session.h"
#include "Utils.h"

namespace Fix::Modules::Writer {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol output handler. Writes outgoing messages and generates heartbeat messages when needed.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Writer
{
public:
    template<typename Context>
    Writer(ORWBuffer& obuf, Context& context)
      : m_obuf(&obuf)
      , m_context(context)
      , m_factory(context)
    {
    }

    bool put(const FIX::Message& msg)
    {
        auto str = msg.toString();
        std::memcpy(m_obuf->wpos(), str.data(), str.size());
        m_obuf->produced(str.size());
        m_latest = std::chrono::steady_clock::now();
        log << level::info << out << '[' << fixdump(str) << ']' << std::endl;
        return false;
    }

    // Send heartbeat message if no other message was sent for the heartbeat interval
    timepoint timeout(timepoint now)
    {
        if (m_context.state() == Context::Session::State::Normal) {
            if (now >= m_latest + std::chrono::seconds(m_context.HBInterval)) {
                auto str = m_factory.heartbeat().toString();
                std::memcpy(m_obuf->wpos(), str.data(), str.size());
                m_obuf->produced(str.size());
                m_latest = now;
                log << level::info << out << '[' << fixdump(str) << ']' << std::endl;
            }
        }
        return m_latest + std::chrono::seconds(m_context.HBInterval);
    }

private:
    ORWBuffer*        m_obuf;
    Context::Session& m_context;
    Factory           m_factory;

    timepoint m_latest = std::chrono::steady_clock::now();
};

} // namespace Fix::Modules::Writer
#endif
