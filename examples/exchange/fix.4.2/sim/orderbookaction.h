#ifndef Paxsim_Examples_Fix42_Sim_OrderBookAction_dot_h
#define Paxsim_Examples_Fix42_Sim_OrderBookAction_dot_h

#include "quickfix/Message.h"

#include "quickfix/fix42/ExecutionReport.h"

#include "session.h"
#include "orderbook.h"

#include "core/streamlog.h"
#include "core/types.h"

#include "control/command.h"

#include <vector>
#include <optional>

#include <json5cpp.h>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

using json    = Json::Value;
using Command = paxsim::control::Command;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Order Book Action module. Relative or Order Book. Onle deals with Control Actions.
//---------------------------------------------------------------------------------------------------------------------
class OrderBookAction : public tag::Control
{
public:
    template<typename Context>
    OrderBookAction(Context& context)
      : m_OContext(context)
      , m_SContext(context)
    {
    }

    // TODO - Until we allow controll only modules, we need to implement this.
    std::vector<std::optional<FIX::Message>> process(const FIX::Message& message)
    {
        return {};
    }

    std::vector<std::optional<FIX::Message>> execute(const Command& command)
    {
        if (const auto* cmd = std::get_if<paxsim::control::order::Command>(&command)) {
            log << level::debug << ctlmark << '[' << *cmd << ']' << std::endl;
            if (const auto* act = std::get_if<paxsim::control::order::Fill>(&cmd->action)) {
                return execute(cmd->selector, *act);
            }
            if (const auto* act = std::get_if<paxsim::control::order::Cancel>(&cmd->action)) {
                return execute(cmd->selector, *act);
            }
        }
        return {};
    }

    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

private:
    template<typename Msg>
    void set_header(Msg& message)
    {
        message.getHeader().set(FIX::SenderCompID(m_SContext.SenderCompID));
        message.getHeader().set(FIX::TargetCompID(m_SContext.TargetCompID));
        message.getHeader().set(FIX::MsgSeqNum(m_SContext.OSequence++));
        message.getHeader().set(FIX::SendingTime::now());
    }

    std::vector<std::optional<FIX::Message>> execute(const paxsim::control::order::Selector& sel,
                                                     const paxsim::control::order::Fill&     fill)
    {
        return {};
    }

    std::vector<std::optional<FIX::Message>> execute(const paxsim::control::order::Selector& sel,
                                                     const paxsim::control::order::Cancel&   cancel)
    {
        return {};
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace fix42::sim
#endif
