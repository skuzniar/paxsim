#ifndef Paxsim_Examples_Fix42_Sim_OrderBook_dot_h
#define Paxsim_Examples_Fix42_Sim_OrderBook_dot_h

#include "quickfix/Message.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/OrderCancelRequest.h"

#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelReject.h"

#include "session.h"
#include "order.h"

#include "core/streamlog.h"
#include "core/types.h"
#include "types.h"

#include <map>
#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

using json = nlohmann::json;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Order Book Context. Provides storage for orders and control commands.
//---------------------------------------------------------------------------------------------------------------------
struct OrderBookContext
{
    OrderBookContext(const json& cfg)
    {
    }

    // Collection of orders
    std::map<Order::ID, Order> OrderBook;
};

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Order Book module.
//---------------------------------------------------------------------------------------------------------------------
class OrderBook : public tag::Control
{
    static constexpr const char* iam = "OrderBook ";

public:
    template<typename Context>
    OrderBook(Context& context)
      : m_OContext(context)
      , m_SContext(context)
    {
    }

    std::vector<std::optional<FIX::Message>> process(FIX::Message message)
    {
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
        // TODO - other types
        return {};
    }

    std::vector<std::optional<FIX::Message>> execute(const json& command)
    {

        // Demonstrate restart using controller command
        if (command["command"] == "stop") {
            return { {} };
        }
        return {};
    }

    static timepoint control_at()
    {
        return std::chrono::steady_clock::now() + std::chrono::seconds(1);
    }

private:
    std::vector<std::optional<FIX::Message>> process(const FIX42::NewOrderSingle& message)
    {
        // auto result = this->accept(mi, m_fixmsg);
        auto order = Order(message);

        if (!m_OContext.OrderBook.insert({ order.clientOrderID(), order }).second) {
            log << level::error << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                << "Duplicate client order ID: " << order.clientOrderID() << std::endl;

            FIX42::ExecutionReport reject;
            set_header(reject);

            reject.set(FIX::ClOrdID(order.clientOrderID()));
            reject.set(FIX::Symbol(order.symbol()));
            reject.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
            reject.set(FIX::OrderID(order.exchangeOrderID()));
            reject.set(FIX::ExecID(order.executionID()));
            reject.set(FIX::ExecType(FIX::ExecType_REJECTED));
            reject.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
            reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
            reject.set(FIX::OrderQty(order.orderQuantity()));
            reject.set(FIX::LeavesQty(order.leavesQuantity()));
            reject.set(FIX::CumQty(order.fillsQuantity()));
            reject.set(FIX::Price(order.price()));
            reject.set(FIX::AvgPx(order.avgPrice()));
            reject.set(FIX::OrdRejReason(FIX::OrdRejReason_DUPLICATE_ORDER));
            reject.set(FIX::Text("Duplicate client order ID: " + order.clientOrderID()));

            log << level::info << hmark << iam << '[' << fixdump(reject.toString()) << ']' << std::endl;
            return { reject };
        }

        FIX42::ExecutionReport report;
        set_header(report);

        report.set(FIX::ClOrdID(order.clientOrderID()));
        report.set(FIX::Symbol(order.symbol()));
        report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        report.set(FIX::OrderID(order.exchangeOrderID()));
        report.set(FIX::ExecID(order.executionID()));
        report.set(FIX::ExecType(FIX::ExecType_NEW));
        report.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        report.set(FIX::OrderQty(order.orderQuantity()));
        report.set(FIX::LeavesQty(order.leavesQuantity()));
        report.set(FIX::CumQty(order.fillsQuantity()));
        report.set(FIX::Price(order.price()));
        report.set(FIX::AvgPx(order.avgPrice()));

        log << level::info << hmark << iam << '[' << fixdump(report.toString()) << ']' << std::endl;
        return { report };
    }

    std::vector<std::optional<FIX::Message>> process(const FIX42::OrderCancelReplaceRequest& message)
    {
        const auto& origid = message.getField(FIX::FIELD::OrigClOrdID);

        auto it = m_OContext.OrderBook.find(origid);
        if (it == m_OContext.OrderBook.end()) {
            FIX42::OrderCancelReject reject;
            set_header(reject);

            reject.set(FIX::OrderID("NONE"));
            reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
            reject.set(FIX::OrigClOrdID(origid));
            reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
            reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REPLACE_REQUEST));
            reject.set(FIX::CxlRejReason(FIX::CxlRejReason_UNKNOWN_ORDER));
            reject.set(FIX::Text("Client order ID: " + origid + " not found."));

            log << level::info << hmark << iam << '[' << fixdump(reject.toString()) << ']' << std::endl;
            return { reject };
        }

        // auto result = this->accept(mi, m_fixmsg);
        auto order = Order(it->second, message);

        if (!m_OContext.OrderBook.insert({ order.clientOrderID(), order }).second) {
            FIX42::OrderCancelReject reject;
            set_header(reject);

            reject.set(FIX::OrderID(order.exchangeOrderID()));
            reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
            reject.set(FIX::OrigClOrdID(origid));
            reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
            reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REPLACE_REQUEST));
            reject.set(FIX::CxlRejReason(FIX::CxlRejReason_DUPLICATE_CL_ORD_ID));
            reject.set(FIX::Text("Duplicate client order ID: " + order.clientOrderID()));
            return { reject };
        }

        FIX42::ExecutionReport report;
        set_header(report);

        report.set(FIX::ClOrdID(order.clientOrderID()));
        report.set(FIX::Symbol(order.symbol()));
        report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        report.set(FIX::OrderID(order.exchangeOrderID()));
        report.set(FIX::ExecID(order.executionID()));
        report.set(FIX::ExecType(FIX::ExecType_REPLACED));
        report.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        report.set(FIX::OrderQty(order.orderQuantity()));
        report.set(FIX::LeavesQty(order.leavesQuantity()));
        report.set(FIX::CumQty(order.fillsQuantity()));
        report.set(FIX::Price(order.price()));
        report.set(FIX::AvgPx(order.avgPrice()));

        log << level::info << hmark << iam << '[' << fixdump(report.toString()) << ']' << std::endl;
        return { report };
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
};

} // namespace fix42::sim
#endif
