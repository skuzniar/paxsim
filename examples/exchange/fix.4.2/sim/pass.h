#ifndef Paxsim_Examples_Fix42_Sim_Pass_dot_h
#define Paxsim_Examples_Fix42_Sim_Pass_dot_h

#include "quickfix/Message.h"

#include "core/streamlog.h"

#include "session.h"
#include "types.h"

#include <vector>
#include <optional>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Pass module. Used as a last module in the HPipeline to forward outgoing messages downstream.
//---------------------------------------------------------------------------------------------------------------------
class Pass
{
public:
    Pass(SessionContext& context)
      : m_context(context)
    {
    }

    std::vector<std::optional<FIX::Message>> process(FIX::Message message)
    {
        const auto& scompid = message.getHeader().getField(FIX::FIELD::SenderCompID);
        const auto& tcompid = message.getHeader().getField(FIX::FIELD::TargetCompID);

        if (scompid == m_context.SenderCompID && tcompid == m_context.TargetCompID) {
            log << level::debug << hmark << '[' << fixdump(message.toString()) << ']' << std::endl;
            return { message };
        }
        return {};
    }

private:
    SessionContext& m_context;
};

} // namespace fix42::sim
#endif
