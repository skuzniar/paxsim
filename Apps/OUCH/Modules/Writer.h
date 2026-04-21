#ifndef OUCH_Modules_Writer_dot_h
#define OUCH_Modules_Writer_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include "OUCH/Context/Session.h"

#include "Common/Config.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH protocol output handler. Outgoing messages must have been created in the output buffer.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Writer
{
public:
    using SequencedData   = Factory::SequencedData;
    using UnsequencedData = Factory::UnsequencedData;

    template<typename Context>
    Writer(ORWBuffer& obuf, const Config& config, Context& context)
      : m_obuf(obuf)
      , m_session(context)
      , m_factory(context)
    {
    }

    template<typename Message>
    bool put(const Message& msg)
    {
        std::memcpy(m_obuf.wpos(), msg.data(), msg.size());
        m_obuf.wmove(msg.size());

        switch (msg.type) {
            case SequencedData::Type:
                log << level::info << out << '[' << reinterpret_cast<const SequencedData&>(msg) << ']' << std::endl;
                break;
            case UnsequencedData::Type:
                log << level::info << out << '[' << reinterpret_cast<const UnsequencedData&>(msg) << ']' << std::endl;
                break;
            default:
                log << level::info << out << '[' << msg << ']' << std::endl;
                break;
        }

        m_latest = std::chrono::steady_clock::now();
        return false;
    }

    // Send heartbeat message if no other message was sent for the heartbeat interval
    timepoint timeout(timepoint now)
    {
        if (m_session.state() == Context::Session::State::Normal) {
            if (now >= m_latest + std::chrono::seconds(m_session.HBInterval)) {
                put(m_factory.heartbeat());
            }
        }
        return m_latest + std::chrono::seconds(m_session.HBInterval);
    }

private:
    ORWBuffer&        m_obuf;
    Context::Session& m_session;
    Factory           m_factory;

    timepoint m_latest = std::chrono::steady_clock::now();
};

} // namespace OUCH::Modules
#endif
