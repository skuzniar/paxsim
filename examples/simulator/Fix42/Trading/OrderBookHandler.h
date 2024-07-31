#ifndef Simulator_Fix42_Trading_OrderBookHandler_dot_h
#define Simulator_Fix42_Trading_OrderBookHandler_dot_h

#include <quickfix/Message.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/OrderCancelReplaceRequest.h>
#include <quickfix/fix42/OrderCancelRequest.h>

#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/OrderCancelReject.h>

#include "Session.h"

#include "Core/Order.h"
#include "Core/Execution.h"
#include "Core/Utils.h"

#include <paxsim/core/streamlog.h>

#include <map>
#include <vector>
#include <optional>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <json5cpp.h>

namespace Fix42::Trading {

using namespace paxsim::core;

using paxsim::core::log;

using json = Json::Value;

namespace bmi = boost::multi_index;

//---------------------------------------------------------------------------------------------------------------------
// Order Book Context. Provides storage for orders and control commands.
//---------------------------------------------------------------------------------------------------------------------
struct OrderBookContext
{
    OrderBookContext(const json& cfg)
    {
    }

    // Collection of orders
    using OrderBook = std::map<Order::ID, Order>;
    OrderBook orderBook;

    // Collection of executions

    // clang-format off
    using ExecutionBook = bmi::multi_index_container <
                  Execution,
                      bmi::indexed_by <
                          bmi::ordered_unique     < bmi::tag<struct idx_exe_id>, bmi::const_mem_fun<Execution, Execution::ID,      &Execution::executionID>>,
                          bmi::ordered_non_unique < bmi::tag<struct idx_ref_id>, bmi::const_mem_fun<Execution, Execution::ID,      &Execution::referenceID>>,
                          bmi::ordered_non_unique < bmi::tag<struct idx_ord_id>, bmi::const_mem_fun<Execution, Execution::OrderID, &Execution::orderID>>
                      >
                  >;
    // clang-format on
    ExecutionBook executionBook;

    std::pair<unsigned, double> fills(const Order& order) const
    {
        const auto& oidx = executionBook.template get<idx_ord_id>();

        unsigned qty = 0;
        double   avg = 0;

        auto range = oidx.equal_range(order.id());
        for (auto itr = range.first; itr != range.second; ++itr) {
            if (itr->active()) {
                qty += itr->quantity();
                avg += itr->quantity() * itr->price();
            }
        }
        return { qty, avg > 0 ? avg / qty : 0 };
    }
};

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Order Book module.
//---------------------------------------------------------------------------------------------------------------------
class OrderBookHandler
{
public:
    template<typename Context>
    OrderBookHandler(Context& context)
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

        // We know there is a filter above. If it produced outgoing message we must let it through
        if (msgtype == FIX::MsgType_ExecutionReport or msgtype == FIX::MsgType_OrderCancelReject) {
            return { message };
        }

        return {};
    }

private:
    std::vector<std::optional<FIX::Message>> process(const FIX42::NewOrderSingle& message)
    {
        auto order = Order(message);

        if (!m_OContext.orderBook.insert({ order.id(), order }).second) {
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
            reject.set(FIX::LeavesQty(order.orderQuantity()));
            reject.set(FIX::CumQty(0));
            reject.set(FIX::Price(order.price()));
            reject.set(FIX::AvgPx(0));
            reject.set(FIX::OrdRejReason(FIX::OrdRejReason_DUPLICATE_ORDER));
            reject.set(FIX::Text("Duplicate client order ID: " + order.clientOrderID()));

            log << level::debug << hmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
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
        report.set(FIX::LeavesQty(order.orderQuantity()));
        report.set(FIX::CumQty(0));
        report.set(FIX::Price(order.price()));
        report.set(FIX::AvgPx(0));

        log << level::debug << hmark << '[' << fixdump(report.toString()) << ']' << std::endl;
        return { report };
    }

    std::vector<std::optional<FIX::Message>> process(const FIX42::OrderCancelReplaceRequest& message)
    {
        const auto& origid = message.getField(FIX::FIELD::OrigClOrdID);

        auto it = m_OContext.orderBook.find(origid);
        if (it == m_OContext.orderBook.end()) {
            FIX42::OrderCancelReject reject;
            set_header(reject);

            reject.set(FIX::OrderID("NONE"));
            reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
            reject.set(FIX::OrigClOrdID(origid));
            reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
            reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REPLACE_REQUEST));
            reject.set(FIX::CxlRejReason(FIX::CxlRejReason_UNKNOWN_ORDER));
            reject.set(FIX::Text("Client order ID: " + origid + " not found."));

            log << level::debug << hmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
            return { reject };
        }

        auto order = Order(it->second, message);

        if (!m_OContext.orderBook.insert({ order.clientOrderID(), order }).second) {
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

        auto [fillqty, avgpx] = m_OContext.fills(order);

        report.set(FIX::ClOrdID(order.clientOrderID()));
        report.set(FIX::OrigClOrdID(origid));
        report.set(FIX::Symbol(order.symbol()));
        report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        report.set(FIX::OrderID(order.exchangeOrderID()));
        report.set(FIX::ExecID(order.executionID()));
        report.set(FIX::ExecType(FIX::ExecType_REPLACED));
        report.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        report.set(FIX::OrderQty(order.orderQuantity()));
        report.set(FIX::LeavesQty(order.orderQuantity() - fillqty));
        report.set(FIX::CumQty(fillqty));
        report.set(FIX::Price(order.price()));
        report.set(FIX::AvgPx(avgpx));

        log << level::debug << hmark << '[' << fixdump(report.toString()) << ']' << std::endl;
        return { report };
    }

    std::vector<std::optional<FIX::Message>> process(const FIX42::OrderCancelRequest& message)
    {
        const auto& origid = message.getField(FIX::FIELD::OrigClOrdID);

        auto it = m_OContext.orderBook.find(origid);
        if (it == m_OContext.orderBook.end()) {
            FIX42::OrderCancelReject reject;
            set_header(reject);

            reject.set(FIX::OrderID("NONE"));
            reject.set(FIX::ClOrdID(message.getField(FIX::FIELD::ClOrdID)));
            reject.set(FIX::OrigClOrdID(origid));
            reject.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(Order::Status::Rejected)));
            reject.set(FIX::CxlRejResponseTo(FIX::CxlRejResponseTo_ORDER_CANCEL_REQUEST));
            reject.set(FIX::CxlRejReason(FIX::CxlRejReason_UNKNOWN_ORDER));
            reject.set(FIX::Text("Client order ID: " + origid + " not found."));

            log << level::debug << hmark << '[' << fixdump(reject.toString()) << ']' << std::endl;
            return { reject };
        }

        auto order = Order(it->second, message);

        FIX42::ExecutionReport report;
        set_header(report);

        auto [fillqty, avgpx] = m_OContext.fills(order);

        report.set(FIX::ClOrdID(order.clientOrderID()));
        report.set(FIX::OrigClOrdID(origid));
        report.set(FIX::Symbol(order.symbol()));
        report.set(FIX::Side(std::underlying_type_t<Order::Side>(order.side())));
        report.set(FIX::OrderID(order.exchangeOrderID()));
        report.set(FIX::ExecID(order.executionID()));
        report.set(FIX::ExecType(FIX::ExecType_CANCELED));
        report.set(FIX::ExecTransType(FIX::ExecTransType_NEW));
        report.set(FIX::OrdStatus(std::underlying_type_t<Order::Status>(order.status())));
        report.set(FIX::OrderQty(order.orderQuantity()));
        report.set(FIX::LeavesQty(order.orderQuantity() - fillqty));
        report.set(FIX::CumQty(fillqty));
        report.set(FIX::Price(order.price()));
        report.set(FIX::AvgPx(avgpx));

        log << level::debug << hmark << '[' << fixdump(report.toString()) << ']' << std::endl;
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

    bool outgoing(const FIX::Message& message)
    {
        const auto& scompid = message.getHeader().getField(FIX::FIELD::SenderCompID);
        const auto& tcompid = message.getHeader().getField(FIX::FIELD::TargetCompID);

        return scompid == m_SContext.SenderCompID && tcompid == m_SContext.TargetCompID;
    }

private:
    OrderBookContext& m_OContext;
    SessionContext&   m_SContext;
};

} // namespace Fix42::Trading
#endif
