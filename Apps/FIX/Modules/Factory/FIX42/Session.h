#ifndef FIX_Modules_Factory_FIX42_Session_dot_h
#define FIX_Modules_Factory_FIX42_Session_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "FIX/Context/Session.h"
#include "FIX/Types.h"

namespace FIX::Modules::Factory::FIX42 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct Session
{
    struct Logon : public IMessage
    {
        static auto type()
        {
            return msg_type::Logon;
        }
    };

    struct Logout : public IMessage
    {
        static auto type()
        {
            return msg_type::Logout;
        }
    };

    struct ResendRequest : public IMessage
    {
        static auto type()
        {
            return msg_type::ResendRequest;
        }
    };

    template<typename Context>
    explicit Session(Context& context)
      : m_session(context)
    {
    }

    auto heartbeat(const std::string& testreqid = "")
    {
        auto omsg = make(msg_type::Heartbeat);
        if (!testreqid.empty()) {
            omsg.push_back_string(tag::TestReqID, testreqid);
        }
        return omsg.commit();
    }

    auto logon()
    {
        auto omsg = make(msg_type::Logon);

        omsg.push_back_int(tag::HeartBtInt, m_session.hbInterval());
        omsg.push_back_int(tag::EncryptMethod, 0);
        return omsg.commit();
    }

    auto logout(const std::string& text = "")
    {
        auto omsg = make(msg_type::Logout);

        if (!text.empty()) {
            omsg.push_back_string(tag::Text, text);
        }
        return omsg.commit();
    }

    auto gapfill()
    {
        auto omsg = make(msg_type::SequenceReset);

        omsg.push_back_char(tag::GapFillFlag, 'Y');
        omsg.push_back_int(tag::NewSeqNo, m_session.iSequence());
        return omsg.commit();
    }

    auto reset()
    {
        auto omsg = make(msg_type::SequenceReset);

        omsg.push_back_char(tag::GapFillFlag, 'N');
        omsg.push_back_int(tag::NewSeqNo, m_session.iSequence());
        return omsg.commit();
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
