#ifndef Simulator_Fix42_Trading_ExecutionBookControl_dot_h
#define Simulator_Fix42_Trading_ExecutionBookControl_dot_h

#include <quickfix/Message.h>
#include <quickfix/fix42/ExecutionReport.h>

#include "Session.h"
#include "OrderBookHandler.h"

#include <paxsim/core/streamlog.h>
#include <paxsim/core/types.h>
#include <paxsim/control/command.h>

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
            if (const auto* act = std::get_if<execution::Bust>(&cmd->action)) {
                log << level::info << ctlmark << '[' << *cmd << ']' << std::endl;
                return execute(cmd->selector, *act);
            }
        }
        return {};
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

    std::vector<std::optional<FIX::Message>> execute(const execution::Selector& selector, const execution::Bust& bust)
    {
        std::vector<std::optional<FIX::Message>> result;

        for (auto& execution : m_OContext.executionBook) {
            if (matches(execution, selector)) {
                log << level::info << hmark << "Bust handling not implemented." << std::endl;
                // result.emplace_back(execute(execution, bust));
            }
        }
        return result;
    }

    std::optional<FIX::Message> execute(const Execution& execution, const execution::Bust& bust)
    {
        return {};
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
