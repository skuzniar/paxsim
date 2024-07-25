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
using namespace paxsim::control;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// Order Book Action module. A relative of Order Book. Deals with Order Control Actions.
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

    // TODO - Until we allow control-only modules, we need to implement this.
    std::vector<std::optional<FIX::Message>> process(const FIX::Message& message)
    {
        return {};
    }

    std::vector<std::optional<FIX::Message>> execute(const Command& command)
    {
        if (const auto* cmd = std::get_if<order::Command>(&command)) {
            log << level::debug << ctlmark << '[' << *cmd << ']' << std::endl;
            if (const auto* act = std::get_if<order::Fill>(&cmd->action)) {
                return execute(cmd->selector, *act);
            }
            if (const auto* act = std::get_if<order::Cancel>(&cmd->action)) {
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

    std::vector<std::optional<FIX::Message>> execute(const order::Selector& selector, const order::Fill& fill)
    {
        std::vector<std::optional<FIX::Message>> result;

        for (const auto& [id, order] : m_OContext.OrderBook) {
            if (matches(order, selector)) {
                log << level::debug << ctlmark << "Filling: " << order << std::endl;
            }
        }
        return result;
    }

    std::vector<std::optional<FIX::Message>> execute(const order::Selector& selector, const order::Cancel& cancel)
    {
        std::vector<std::optional<FIX::Message>> result;

        for (const auto& [id, order] : m_OContext.OrderBook) {
            if (matches(order, selector)) {
                log << level::debug << ctlmark << "Canceling: " << order << std::endl;
            }
        }
        return result;
    }

    bool matches(const Order& order, const order::Selector& selector)
    {
        return true;
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace fix42::sim
#endif
