#ifndef Paxsim_Examples_Fix42_Sim_ExecutionBookControl_dot_h
#define Paxsim_Examples_Fix42_Sim_ExecutionBookControl_dot_h

#include "quickfix/Message.h"

#include "quickfix/fix42/ExecutionReport.h"

#include "Session.h"
#include "OrderBookHandler.h"

#include "core/streamlog.h"
#include "core/types.h"

#include "control/command.h"

#include <vector>
#include <optional>

#include <json5cpp.h>

namespace Fix42::Trading {

using namespace paxsim::core;
using namespace paxsim::control;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// Order Book Control module. A relative of Order Book Handler. Deals with Order Book Commands.
//---------------------------------------------------------------------------------------------------------------------
class ExecutionBookControl : public tag::Control
{
public:
    template<typename Context>
    ExecutionBookControl(Context& context)
      : m_OContext(context)
      , m_SContext(context)
    {
    }

    // TODO - Until we allow control-only modules, we need to implement this.
    std::vector<std::optional<FIX::Message>> process(const FIX::Message& message)
    {
        return {};
    }

    std::vector<std::optional<FIX::Message>> execute(const Command& command)
    {
        if (const auto* cmd = std::get_if<execution::Command>(&command)) {
            log << level::info << ctlmark << '[' << *cmd << ']' << std::endl;
            if (const auto* act = std::get_if<execution::Bust>(&cmd->action)) {
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

    std::vector<std::optional<FIX::Message>> execute(const execution::Selector& selector, const execution::Bust& fill)
    {
        std::vector<std::optional<FIX::Message>> result;

        for (auto it = m_OContext.executionBook.cbegin(); it != m_OContext.executionBook.cend();) {
            const auto& execution = *it;
            if (matches(execution, selector)) {
                log << level::info << hmark << "Bust handling not implemented." << std::endl;
                ++it;
            }
        }
        return result;
    }

    bool matches(const Execution& execution, const execution::Selector& selector)
    {
        return execution.id() == selector.params.id or selector.params.id == "*";
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace Fix42::Trading
#endif
