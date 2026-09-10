#ifndef OUCH_OUCH50_Factory_dot_h
#define OUCH_OUCH50_Factory_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/OUCH50/ServerHeartbeat.h"
#include "OUCH/OUCH50/ClientHeartbeat.h"

#include "OUCH/OUCH50/LoginRequest.h"
#include "OUCH/OUCH50/LoginAccepted.h"
#include "OUCH/OUCH50/LoginRejected.h"
#include "OUCH/OUCH50/LogoutRequest.h"
#include "OUCH/OUCH50/AccountQuery.h"
#include "OUCH/OUCH50/AccountQueryResponse.h"

#include "OUCH/OUCH50/EnterOrder.h"
#include "OUCH/OUCH50/ReplaceOrder.h"
#include "OUCH/OUCH50/CancelOrder.h"

#include "OUCH/OUCH50/OrderAccepted.h"
#include "OUCH/OUCH50/OrderRejected.h"

#include "OUCH/OUCH50/OrderReplaced.h"

#include "OUCH/OUCH50/OrderCancelled.h"
#include "OUCH/OUCH50/CancelRejected.h"

#include "OUCH/OUCH50/OrderExecuted.h"
#include "OUCH/OUCH50/TradeBroken.h"

#include "OUCH/Context/Session.h"
#include "OUCH/Types.h"

#include "Common/Config.h"

namespace OUCH::OUCH50 {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class Factory
{
    using Config = Common::Config;

public:
    //-----------------------------------------------------------------------------------------------------------------
    // Expose version specific types
    //-----------------------------------------------------------------------------------------------------------------
    using PacketHeader    = OUCH50::PacketHeader;
    using SequencedData   = OUCH50::SequencedData;
    using UnsequencedData = OUCH50::UnsequencedData;

    using LoginRequest  = OUCH50::LoginRequest;
    using LoginAccepted = OUCH50::LoginAccepted;
    using LoginRejected = OUCH50::LoginRejected;
    using LogoutRequest = OUCH50::LogoutRequest;

    using AccountQuery         = OUCH50::AccountQuery;
    using AccountQueryResponse = OUCH50::AccountQueryResponse;

    using EnterOrder   = OUCH50::EnterOrder;
    using ReplaceOrder = OUCH50::ReplaceOrder;
    using CancelOrder  = OUCH50::CancelOrder;

    template<typename Context>
    explicit Factory(Context& context)
      : m_session(context)
    {
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Message arrival.
    //-----------------------------------------------------------------------------------------------------------------
    static std::pair<bool, const PacketHeader&> message(const char* buff, std::size_t size)
    {
        const auto* header = reinterpret_cast<const PacketHeader*>(buff);
        if (size >= sizeof(PacketHeader)) {
            std::size_t length = header->size();
            if (size >= length) {
                return { true, *header };
            }
        }
        return { false, *header };
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Heartbeat and login sequence handling
    //-----------------------------------------------------------------------------------------------------------------
    static auto clientHeartbeat()
    {
        return ClientHeartbeat();
    }

    static auto serverHeartbeat()
    {
        return ServerHeartbeat();
    }

    auto loginRequest()
    {
        return LoginRequest(m_session.UserName, m_session.Password);
    }

    static auto loginAccept()
    {
        return LoginAccepted();
    }

    static auto logoutRequest()
    {
        return LogoutRequest();
    }

    static auto accountQuery()
    {
        return AccountQuery();
    }

    auto accountQueryResponse()
    {
        AccountQueryResponse response;
        response.timestamp      = Timestamp::now();
        response.nextUserRefNum = m_session.iSequence();
        return response;
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Enter Order handling
    //-----------------------------------------------------------------------------------------------------------------
    static auto clordID(const EnterOrder& msg)
    {
        return msg.userRefNum;
    }

    static Order order(const EnterOrder& msg)
    {
        return Order(msg.userRefNum, msg.buySellIndicator, msg.orderBook, msg.quantity, msg.price);
    }

    auto accept(const EnterOrder& msg, const Order& order)
    {
        OrderAccepted omsg;

        omsg.timestamp                    = Timestamp::now();
        omsg.userRefNum                   = msg.userRefNum;
        omsg.price                        = msg.price;
        omsg.orderReferenceNumber         = order.orderID();
        omsg.buySellIndicator             = msg.buySellIndicator;
        omsg.orderBook                    = msg.orderBook;
        omsg.quantity                     = msg.quantity;
        omsg.user                         = msg.user;
        omsg.executionWithinFirm          = msg.executionWithinFirm;
        omsg.investmentDecisionWithinFirm = msg.investmentDecisionWithinFirm;
        omsg.clientIdentifier             = msg.clientIdentifier;
        omsg.partyRoleQualifier           = msg.partyRoleQualifier;
        omsg.capacity                     = msg.capacity;
        omsg.algoIndicator                = msg.algoIndicator;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const EnterOrder& msg, RejectReason reason)
    {
        OrderRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const EnterOrder& msg, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        OrderRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Replace Order handling
    //-----------------------------------------------------------------------------------------------------------------
    static auto clordID(const ReplaceOrder& msg)
    {
        return msg.origUserRefNum;
    }

    static Order order(const ReplaceOrder& msg, const Order& o)
    {
        Order oo(msg.newUserRefNum, o.side(), o.bookID(), msg.quantity, msg.price);
        return oo;
    }

    auto accept(const ReplaceOrder& msg, const Order& order)
    {
        OrderReplaced omsg;

        omsg.timestamp            = Timestamp::now();
        omsg.origUserRefNum       = msg.origUserRefNum;
        omsg.newUserRefNum        = msg.newUserRefNum;
        omsg.price                = order.price();
        omsg.orderReferenceNumber = order.orderID();
        omsg.buySellIndicator     = order.side();
        omsg.orderBook            = order.bookID();
        omsg.quantity             = order.quantity();
        omsg.user                 = msg.user;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const ReplaceOrder& msg, RejectReason reason)
    {
        OrderRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.newUserRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const ReplaceOrder& msg, const Order& order, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        OrderRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.newUserRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Cancel Order handling
    //-----------------------------------------------------------------------------------------------------------------
    static auto clordID(const CancelOrder& msg)
    {
        return msg.userRefNum;
    }

    auto accept(const CancelOrder& msg, const Order& order)
    {
        OrderCancelled omsg;

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::CancelReason::UserRequested;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const CancelOrder& msg, RejectReason reason)
    {
        CancelRejected omsg;

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto reject(const CancelOrder& msg, const Order& order, const Config::Table& params)
    {
        log << level::trace << ts << ' ' << here << ' ' << '[' << msg << ']' << std::endl;
        CancelRejected omsg;

        int reason = params["Error"];

        omsg.timestamp  = Timestamp::now();
        omsg.userRefNum = msg.userRefNum;
        omsg.reason     = reason;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Unsolicited Order cancel handling
    //-----------------------------------------------------------------------------------------------------------------
    auto cancel(const Order& order, const Config::Table& params)
    {
        OrderCancelled omsg;

        std::string reason = params["Reason"];

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::template enum_cast<OrderCancelled::CancelReason>(reason);

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto cancel(const Order& order)
    {
        OrderCancelled omsg;

        omsg.timestamp         = Timestamp::now();
        omsg.userRefNum        = order.clordID();
        omsg.decrementQuantity = 0;
        omsg.reason            = OrderCancelled::CancelReason::UserRequested;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    //-----------------------------------------------------------------------------------------------------------------
    // Trade and trade cancel handling
    //-----------------------------------------------------------------------------------------------------------------
    auto report(const Fill& fill, const Order& order)
    {
        OrderExecuted omsg;

        omsg.timestamp           = Timestamp::now();
        omsg.userRefNum          = order.clordID();
        omsg.executedQuantity    = fill.quantity();
        omsg.executionPrice      = fill.price();
        omsg.liquidityFlag       = LiquidityFlag::AuctionTrade;
        omsg.matchNumber         = fill.executionID();
        omsg.contraFirm          = std::string_view("PaxFrm");
        omsg.tradingMode         = TradingMode::UnscheduledAuction;
        omsg.transactionCategory = TransactionCategory::NoneApply;
        omsg.algoIndicator       = AlgoIndicator::Algo;
        omsg.liquidityAttributes =
            LiquidityAttributes(LiquidityIndicator::Auction, LiquidityInternalized::Internalized, LiquidityTopOfBook::TopOfBook, LiquiditySelfTrade::SelfTrade);
        omsg.lastMarket = Market::DHEL;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

    auto cancel(const Fill& fill, const Order& order, const Config::Table& params)
    {
        TradeBroken omsg;

        std::string reason = params["Reason"];

        omsg.timestamp           = Timestamp::now();
        omsg.userRefNum          = order.clordID();
        omsg.matchNumber         = fill.executionID();
        omsg.reason              = TradeBroken::enum_cast<TradeBroken::BreakReason>(reason);
        omsg.tradingMode         = TradingMode::ContinuousTrading;
        omsg.transactionCategory = TransactionCategory::NoneApply;
        omsg.algoIndicator       = AlgoIndicator::Algo;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return omsg;
    }

private:
    Context::Session& m_session;
};

} // namespace OUCH::OUCH50
#endif
