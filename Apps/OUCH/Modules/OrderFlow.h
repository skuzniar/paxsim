#ifndef OUCH_Modules_OrderFlow_dot_h
#define OUCH_Modules_OrderFlow_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

#include "OUCH/Context/Session.h"
#include "OUCH/Context/OrderBook.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH protocol order handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderFlow
{
public:
    using UnsequencedData = Factory::UnsequencedData;
    using EnterOrder      = Factory::EnterOrder;
    using ReplaceOrder    = Factory::ReplaceOrder;
    using CancelOrder     = Factory::CancelOrder;

    template<typename Context>
    OrderFlow(const Config& config, Context& context)
      : m_session(context)
      , m_orderbook(context)
      , m_factory(context)
    {
    }

    template<typename Message, typename Next>
    bool put(const Message& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;

        if (msg.type == UnsequencedData::Type) {
            const auto& header = reinterpret_cast<const UnsequencedData&>(msg);

            if (header.type == EnterOrder::Type) {
                return process(reinterpret_cast<const EnterOrder&>(header), next);
            }
            if (header.type == ReplaceOrder::Type) {
                return process(reinterpret_cast<const ReplaceOrder&>(header), next);
            }
            if (header.type == CancelOrder::Type) {
                return process(reinterpret_cast<const CancelOrder&>(header), next);
            }
        }
        return true;
    }

private:
    template<typename Next>
    bool process(const EnterOrder& msg, Next& next)
    {
        log << level::debug << iflow << '[' << msg << ']' << std::endl;

        if (msg.userRefNum > m_session.iSequence()) {

            auto o = m_factory.order(msg);
            if (m_orderbook.orders.insert(o).second) {
                m_session.iSequence(msg.userRefNum + 1);
                return next.put(m_factory.accept(msg, o));
            }
            log << level::error << ts << here << ' ' << "Unable to enter order with client order ID: " << Factory::clordID(msg) << std::endl;
            return next.put(m_factory.reject(msg, { 99 }));
        }
        log << level::info << ts << here << ' ' << "Ignoring order with user reference number: " << msg.userRefNum << std::endl;
        return false;
    }

    template<typename Next>
    bool process(const ReplaceOrder& msg, Next& next)
    {
        log << level::debug << iflow << '[' << msg << ']' << std::endl;

        if (msg.newUserRefNum > m_session.iSequence()) {
            auto& idx = m_orderbook.orders.get<Context::OrderBook::clordid>();

            auto itr = idx.find(Factory::clordID(msg));
            if (itr == idx.end()) {
                log << level::error << ts << here << ' ' << "Unknown client order ID: " << Factory::clordID(msg) << std::endl;
                return next.put(m_factory.reject(msg, { 99 }));
            }

            auto o = m_factory.order(msg, *itr);
            o.status(Order::Status::Replaced);
            idx.erase(itr);
            if (m_orderbook.orders.insert(o).second) {
                m_session.iSequence(msg.newUserRefNum + 1);
                return next.put(m_factory.accept(msg, o));
            }
        }
        log << level::info << ts << here << ' ' << "Ignoring order replace with user reference number: " << msg.newUserRefNum << std::endl;
        return false;
    }

    template<typename Next>
    bool process(const CancelOrder& msg, Next& next)
    {
        log << level::debug << iflow << '[' << msg << ']' << std::endl;

        if (msg.userRefNum > m_session.iSequence()) {
            auto& idx = m_orderbook.orders.get<Context::OrderBook::clordid>();
            auto  itr = idx.find(Factory::clordID(msg));
            if (itr == idx.end()) {
                log << level::error << ts << here << ' ' << "Unknown client order ID: " << Factory::clordID(msg) << std::endl;
                return next.put(m_factory.reject(msg, { 99 }));
            }

            itr->status(Order::Status::Canceled);
            auto ret = next.put(m_factory.accept(msg, *itr));
            idx.erase(itr);

            return ret;
        }
        log << level::info << ts << here << ' ' << "Ignoring order cancel with user reference number: " << msg.userRefNum << std::endl;
        return false;
    }

    Context::Session&   m_session;
    Context::OrderBook& m_orderbook;
    Factory             m_factory;
};

} // namespace OUCH::Modules
#endif
