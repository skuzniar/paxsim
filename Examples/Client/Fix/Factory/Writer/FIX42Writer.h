#ifndef Factory_Writer_FIX42Writer_dot_h
#define Factory_Writer_FIX42Writer_dot_h

#include "Context/Session/Session.h"

#include <quickfix/fix42/Heartbeat.h>

namespace Fix::Factory::Writer {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct FIX42Writer
{
    using Heartbeat = FIX42::Heartbeat;

    template<typename Context>
    explicit FIX42Writer(Context& context)
      : m_session(context)
    {
    }

    FIX42::Heartbeat heartbeat(const std::string& testreqid = "")
    {
        FIX42::Heartbeat heartbeat;
        heartbeat.getHeader().set(FIX::SenderCompID(m_session.senderCompID()));
        heartbeat.getHeader().set(FIX::TargetCompID(m_session.targetCompID()));
        heartbeat.getHeader().set(FIX::MsgSeqNum(m_session.nextOSequence()));
        heartbeat.getHeader().set(FIX::SendingTime());

        if (!testreqid.empty()) {
            heartbeat.set(FIX::TestReqID(testreqid));
        }
        return heartbeat;
    }

    Context::Session& m_session;
};

} // namespace Fix::Factory::Writer
#endif
