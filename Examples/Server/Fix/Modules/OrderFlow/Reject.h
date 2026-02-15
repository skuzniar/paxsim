#ifndef Modules_OrderFlow_Reject_dot_h
#define Modules_OrderFlow_Reject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context/Session/Session.h"
#include "Context/OrderBook/OrderBook.h"
#include "Utils.h"

#include <quickfix/Message.h>

namespace Fix::Modules::OrderFlow {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Reject
{
public:
    template<typename Context>
    explicit Reject(Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_factory(context)
    {
    }

    template<typename Next>
    bool put(const FIX::Message& msg, Next& next)
    {
        log << level::debug << vmark << '[' << fixdump(msg.toString()) << ']' << std::endl;

        const auto& msgtype = msg.getHeader().getField(FIX::FIELD::MsgType);
        if (msgtype == FIX::MsgType_NewOrderSingle) {
            return process(typename Factory::NewOrderSingle(msg), next);
        }
        if (msgtype == FIX::MsgType_OrderCancelReplaceRequest) {
            return process(typename Factory::OrderCancelReplaceRequest(msg), next);
        }
        if (msgtype == FIX::MsgType_OrderCancelRequest) {
            return process(typename Factory::OrderCancelRequest(msg), next);
        }
        return true;
    }

private:
    using Order   = Context::Order;
    using clordid = Context::OrderBook::clordid;

    template<typename Next>
    bool process(const Factory::NewOrderSingle& msg, Next& next)
    {
        static const auto& config = m_orderbook.config()["OrderFlow.Reject.Enter"];

        if (int quantity = std::stoi(msg.getField(FIX::FIELD::OrderQty)); quantity == config["Quantity"]) {
            int code = config["ErrorCode"];
            if (code == 0) {
                code = FIX::OrdRejReason_ORDER_EXCEEDS_LIMIT;
            }
            std::string message = config["Message"];
            if (message.empty()) {
                message = "Order quantity: " + std::to_string(quantity) + " triggered reject";
            }
            log << level::info << ts << here << ' ' << code << ' ' << message << std::endl;
            next.put(m_factory.reject(msg, code, message));
            return false;
        }
        return true;
    }

    template<typename Next>
    bool process(const Factory::OrderCancelReplaceRequest& msg, Next& next)
    {
        static const auto& config = m_orderbook.config()["OrderFlow.Reject.Replace"];

        if (int quantity = std::stoi(msg.getField(FIX::FIELD::OrderQty)); quantity == config["Quantity"]) {
            int code = config["ErrorCode"];
            if (code == 0) {
                code = FIX::OrdRejReason_ORDER_EXCEEDS_LIMIT;
            }
            std::string message = config["Message"];
            if (message.empty()) {
                message = "Order quantity: " + std::to_string(quantity) + " triggered reject";
            }
            log << level::info << ts << here << ' ' << code << ' ' << message << std::endl;

            auto& idx = m_orderbook.orders.get<clordid>();

            auto itr = idx.find(msg.getField(FIX::FIELD::OrigClOrdID));
            if (itr == idx.end()) {
                next.put(m_factory.reject(msg, code, message));
            } else {
                next.put(m_factory.reject(msg, *itr, code, message));
            }
            return false;
        }
        return true;
    }

    template<typename Next>
    bool process(const Factory::OrderCancelRequest& msg, Next& next)
    {
        auto& idx = m_orderbook.orders.get<clordid>();
        auto  itr = idx.find(msg.getField(FIX::FIELD::OrigClOrdID));

        if (itr != idx.end()) {
            static const auto& config = m_orderbook.config()["OrderFlow.Reject.Cancel"];

            if (itr->quantity() == config["Quantity"]) {
                int code = config["ErrorCode"];
                if (code == 0) {
                    code = FIX::OrdRejReason_ORDER_EXCEEDS_LIMIT;
                }
                std::string message = config["Message"];
                if (message.empty()) {
                    message = "Order quantity: " + std::to_string(itr->quantity()) + " triggered reject";
                }
                log << level::info << ts << here << ' ' << code << ' ' << message << std::endl;

                next.put(m_factory.reject(msg, *itr, code, message));
                return false;
            }
        }
        return true;
    }

    Context::Session&   m_session;
    Context::OrderBook& m_orderbook;
    Factory             m_factory;
};

} // namespace Fix::Modules::OrderFlow
#endif
