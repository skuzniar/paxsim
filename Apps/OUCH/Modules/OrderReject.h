#ifndef OUCH_Modules_OrderReject_dot_h
#define OUCH_Modules_OrderReject_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/OrderBook.h"

#include "Common/Params/OrderReject.h"

namespace OUCH::Modules {

using namespace Common;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Order reject handling module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class OrderReject
{
public:
    using PacketHeader    = Factory::PacketHeader;
    using UnsequencedData = Factory::UnsequencedData;
    using EnterOrder      = Factory::EnterOrder;
    using ReplaceOrder    = Factory::ReplaceOrder;
    using CancelOrder     = Factory::CancelOrder;

    template<typename Context>
    OrderReject(const Config& config, Context& context)
      : m_orderbook(context)
      , m_params(context)
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
    using clordid = Context::OrderBook::clordid;

    template<typename Next>
    bool process(const EnterOrder& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        int quantity = msg.quantity;
        if (auto pit = m_params.enter.find(quantity); pit != m_params.enter.end()) {
            next.put(m_factory.reject(msg, pit->second));
            return false;
        }
        return true;
    }

    template<typename Next>
    bool process(const ReplaceOrder& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        auto& idx = m_orderbook.orders.get<clordid>();

        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            int quantity = itr->quantity();
            if (auto pit = m_params.replace.find(quantity); pit != m_params.replace.end()) {
                next.put(m_factory.reject(msg, *itr, pit->second));
                return false;
            }
        }
        return true;
    }

    template<typename Next>
    bool process(const CancelOrder& msg, Next& next)
    {
        log << level::trace << ts << here << std::endl;
        auto& idx = m_orderbook.orders.get<clordid>();

        if (auto itr = idx.find(Factory::clordID(msg)); itr != idx.end()) {
            int quantity = itr->quantity();
            if (auto pit = m_params.cancel.find(quantity); pit != m_params.cancel.end()) {
                next.put(m_factory.reject(msg, *itr, pit->second));
                return false;
            }
        }
        return true;
    }

    Context::OrderBook&  m_orderbook;
    Params::OrderReject& m_params;
    Factory              m_factory;
};

} // namespace OUCH::Modules
#endif
