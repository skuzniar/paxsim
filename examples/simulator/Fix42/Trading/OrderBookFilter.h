#ifndef Simulator_Fix42_Trading_OrderBookFilter_dot_h
#define Simulator_Fix42_Trading_OrderBookFilter_dot_h

#include <quickfix/Message.h>
#include <quickfix/fix42/ExecutionReport.h>

#include "Session.h"
#include "OrderBookHandler.h"

#include "Fix42/Common/Factory.h"

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
// Order Book Filter module. Looks at messages before they hit Order Book Handler. Deals with Order Book Commands.
//---------------------------------------------------------------------------------------------------------------------
class OrderBookFilter : public tag::Control
{
public:
    template<typename Context>
    OrderBookFilter(Context& context)
      : m_OContext(context)
      , m_SContext(context)
    {
    }

    // Apply filter command to the incoming message
    std::vector<std::optional<FIX::Message>> process(FIX::Message message)
    {
        if (m_command) {
            const auto& msgtype = message.getHeader().getField(FIX::FIELD::MsgType);

            if (msgtype == FIX::MsgType_NewOrderSingle) {
                return process(FIX42::NewOrderSingle(message));
            }
            if (msgtype == FIX::MsgType_OrderCancelReplaceRequest) {
                return process(FIX42::OrderCancelReplaceRequest(message));
            }
            if (msgtype == FIX::MsgType_OrderCancelRequest) {
                return process(FIX42::OrderCancelRequest(message));
            }
        }
        return { message };
    }

    std::vector<std::optional<FIX::Message>> execute(const Command& command)
    {
        if (const auto* cmd = std::get_if<order::Command>(&command)) {
            if (const auto* act = std::get_if<order::Reject>(&cmd->action)) {
                log << level::info << ctlmark << '[' << *cmd << ']' << std::endl;
                m_command = *cmd;
            }
        }
        return {};
    }

private:
    std::vector<std::optional<FIX::Message>> process(const FIX42::NewOrderSingle& message)
    {
        const auto& action = std::get<order::Reject>(m_command->action);

        FIX42::ExecutionReport reject;
        set_header(reject);

        // For simplicity
        auto order = Common::Factory::order(message);

        reject.set(FIX::ClOrdID(order.clientOrderID()));
        reject.set(FIX::Symbol(order.symbol()));
        reject.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        reject.set(FIX::OrderID(order.exchangeOrderID()));
        reject.set(FIX::ExecID(order.executionID()));
        reject.set(FIX::ExecType(FIX::ExecType_REJECTED));
        reject.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
        reject.set(FIX::OrderQty(order.quantity()));
        reject.set(FIX::LeavesQty(order.quantity()));
        reject.set(FIX::CumQty(0));
        reject.set(FIX::Price(order.price()));
        reject.set(FIX::AvgPx(0));
        reject.set(FIX::OrdRejReason(action.params.code));
        reject.set(FIX::Text(action.params.text));

        log << level::debug << vmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
        m_command.reset();
        return { reject };
    }

    std::vector<std::optional<FIX::Message>> process(const FIX42::OrderCancelReplaceRequest& message)
    {
        const auto& action = std::get<order::Reject>(m_command->action);

        FIX42::OrderCancelReject reject;
        set_header(reject);

        const auto& origid = message.getField(FIX::FIELD::OrigClOrdID);
        auto        it     = m_OContext.orderBook.find(origid);
        if (it == m_OContext.orderBook.end()) {
            reject.set(FIX::OrderID("NONE"));
        } else {
            reject.set(FIX::OrderID(it->second.exchangeOrderID()));
        }

        reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
        reject.set(FIX::OrigClOrdID(origid));
        reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
        reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REPLACE_REQUEST));
        reject.set(FIX::CxlRejReason(action.params.code));
        reject.set(FIX::Text(action.params.text));

        log << level::debug << hmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
        m_command.reset();
        return { reject };
    }

    std::vector<std::optional<FIX::Message>> process(const FIX42::OrderCancelRequest& message)
    {
        const auto& action = std::get<order::Reject>(m_command->action);

        FIX42::OrderCancelReject reject;
        set_header(reject);

        const auto& origid = message.getField(FIX::FIELD::OrigClOrdID);
        auto        it     = m_OContext.orderBook.find(origid);
        if (it == m_OContext.orderBook.end()) {
            reject.set(FIX::OrderID("NONE"));
        } else {
            reject.set(FIX::OrderID(it->second.exchangeOrderID()));
        }

        reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
        reject.set(FIX::OrigClOrdID(origid));
        reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
        reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REQUEST));
        reject.set(FIX::CxlRejReason(action.params.code));
        reject.set(FIX::Text(action.params.text));

        log << level::debug << hmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
        m_command.reset();
        return { reject };
    }

    template<typename Msg>
    void set_header(Msg& message)
    {
        message.getHeader().set(FIX::SenderCompID(m_SContext.SenderCompID));
        message.getHeader().set(FIX::TargetCompID(m_SContext.TargetCompID));
        message.getHeader().set(FIX::MsgSeqNum(m_SContext.OSequence++));
        message.getHeader().set(FIX::SendingTime::now());
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;

    std::optional<order::Command> m_command;
};

} // namespace Fix42::Trading
#endif
