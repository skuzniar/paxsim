#ifndef Paxsim_Examples_Fix42_Sim_OrderBookControl_dot_h
#define Paxsim_Examples_Fix42_Sim_OrderBookControl_dot_h

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
class OrderBookControl : public tag::Control
{
public:
    template<typename Context>
    OrderBookControl(Context& context)
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
            log << level::info << ctlmark << '[' << *cmd << ']' << std::endl;
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

        for (auto& [_, order] : m_OContext.orderBook) {
            if (matches(order, selector)) {

                order.execute(fill.params.quantity, fill.params.price);

                FIX42::ExecutionReport report;
                set_header(report);

                auto [fillqty, avgpx] = m_OContext.fills(order);

                report.set(FIX::ClOrdID(order.clientOrderID()));
                report.set(FIX::Symbol(order.symbol()));
                report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
                report.set(FIX::OrderID(order.exchangeOrderID()));
                report.set(FIX::ExecID(order.executionID()));
                report.set(
                    FIX::ExecType(order.orderQuantity() == fillqty ? FIX::ExecType_FILL : FIX::ExecType_PARTIAL_FILL));
                report.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
                report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
                report.set(FIX::OrderQty(order.orderQuantity()));
                report.set(FIX::LeavesQty(order.orderQuantity() - fillqty));
                report.set(FIX::CumQty(fillqty));
                report.set(FIX::Price(order.price()));
                report.set(FIX::AvgPx(avgpx));

                log << level::debug << hmark << '[' << fixdump(report.toString()) << ']' << std::endl;
                result.emplace_back(report);
            }
        }
        return result;
    }

    std::vector<std::optional<FIX::Message>> execute(const order::Selector& selector, const order::Cancel& cancel)
    {
        std::vector<std::optional<FIX::Message>> result;

        for (auto it = m_OContext.orderBook.cbegin(); it != m_OContext.orderBook.cend();) {
            const auto& [id, order] = *it;
            if (matches(order, selector)) {
                FIX42::ExecutionReport report;
                set_header(report);

                auto [fillqty, avgpx] = m_OContext.fills(order);

                report.set(FIX::ClOrdID(order.clientOrderID()));
                report.set(FIX::Symbol(order.symbol()));
                report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
                report.set(FIX::OrderID(order.exchangeOrderID()));
                report.set(FIX::ExecID(order.executionID()));
                report.set(FIX::ExecType(FIX::ExecType_CANCELED));
                report.set(FIX::ExecTransType(FIX::ExecTransType_CANCEL));
                report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
                report.set(FIX::OrderQty(order.orderQuantity()));
                report.set(FIX::LeavesQty(order.orderQuantity() - fillqty));
                report.set(FIX::CumQty(fillqty));
                report.set(FIX::Price(order.price()));
                report.set(FIX::AvgPx(avgpx));

                report.set(FIX::Text(cancel.params.text));

                log << level::debug << hmark << '[' << fixdump(report.toString()) << ']' << std::endl;
                result.emplace_back(report);
                it = m_OContext.orderBook.erase(it);
            } else {
                ++it;
            }
        }
        return result;
    }

    bool matches(const Order& order, const order::Selector& selector)
    {
        if (!selector.params.id.empty()) {
            return (order.clientOrderID() == selector.params.id) or selector.params.id == "*";
        }
        return (order.orderQuantity() == selector.params.quantity and order.price() == selector.params.price);
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace Fix42::Trading
#endif
