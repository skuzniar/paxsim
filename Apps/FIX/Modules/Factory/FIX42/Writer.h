#ifndef FIX_Modules_Factory_FIX42_Writer_dot_h
#define FIX_Modules_Factory_FIX42_Writer_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Types.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct Writer
{
    template<typename Context>
    explicit Writer(Context& context)
      : m_session(context)
    {
    }

    OMessage heartbeat(const std::string& testreqid = "")
    {
        auto omsg = make(msg_type::Heartbeat);
        if (!testreqid.empty()) {
            omsg.push_back_string(tag::TestReqID, testreqid);
        }
        omsg.commit();

        return omsg;
    }

private:
    OMessage make(const char* msgtype)
    {
        OMessage omsg(m_buff.data(), m_buff.size(), m_session.beginString());

        omsg.push_back_string(tag::MsgType, msgtype);
        omsg.push_back_string(tag::SenderCompID, m_session.senderCompID());
        omsg.push_back_string(tag::TargetCompID, m_session.targetCompID());
        omsg.push_back_int(tag::MsgSeqNum, m_session.nextOSequence());
        omsg.push_back_timestamp(tag::SendingTime, std::chrono::system_clock::now());

        return omsg;
    }

    Context::Session&      m_session;
    std::array<char, 1024> m_buff;
};

} // namespace FIX::Modules::Factory::FIX42
#endif
