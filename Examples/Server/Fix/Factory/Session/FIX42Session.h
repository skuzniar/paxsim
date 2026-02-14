#ifndef Factory_Session_FIX42Session_dot_h
#define Factory_Session_FIX42Session_dot_h

#include "Context/Session/Session.h"

#include <quickfix/fix42/Heartbeat.h>
#include "quickfix/fix42/Logon.h"
#include <quickfix/fix42/Logout.h>
#include <quickfix/fix42/ResendRequest.h>
#include <quickfix/fix42/SequenceReset.h>

namespace Fix::Factory::Session {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct FIX42Session
{
    static constexpr auto BeginString = FIX::BeginString_FIX42;

    using Heartbeat     = FIX42::Heartbeat;
    using Logon         = FIX42::Logon;
    using Logout        = FIX42::Logout;
    using ResendRequest = FIX42::ResendRequest;
    using SequenceReset = FIX42::SequenceReset;

    template<typename Context>
    explicit FIX42Session(Context& context)
      : m_session(context)
    {
    }

    FIX42::Heartbeat heartbeat(const std::string& testreqid = "")
    {
        auto omsg = make<FIX42::Heartbeat>();

        if (!testreqid.empty()) {
            omsg.set(FIX::TestReqID(testreqid));
        }
        return omsg;
    }

    FIX42::Logon logon()
    {
        auto omsg = make<FIX42::Logon>();

        omsg.set(FIX::HeartBtInt(m_session.hbInterval()));
        omsg.set(FIX::EncryptMethod(0));
        return omsg;
    }

    FIX42::Logout logout(const std::string& text = "")
    {
        auto omsg = make<FIX42::Logout>();

        if (!text.empty()) {
            omsg.set(FIX::Text(text));
        }
        return omsg;
    }

    FIX42::Logout gapfill()
    {
        auto omsg = make<FIX42::SequenceReset>();

        omsg.set(FIX::GapFillFlag(true));
        omsg.set(FIX::NewSeqNo(m_session.iSequence()));
        return omsg;
    }

    FIX42::Logout reset()
    {
        auto omsg = make<FIX42::SequenceReset>();

        omsg.set(FIX::GapFillFlag(false));
        omsg.set(FIX::NewSeqNo(m_session.iSequence()));
        return omsg;
    }

private:
    template<typename Message>
    Message make()
    {
        Message msg;
        msg.getHeader().set(FIX::SenderCompID(m_session.senderCompID()));
        msg.getHeader().set(FIX::TargetCompID(m_session.targetCompID()));
        msg.getHeader().set(FIX::MsgSeqNum(m_session.nextOSequence()));
        msg.getHeader().set(FIX::SendingTime());
        return std::move(msg);
    }

    Context::Session& m_session;
};

} // namespace Fix::Factory::Session
#endif
