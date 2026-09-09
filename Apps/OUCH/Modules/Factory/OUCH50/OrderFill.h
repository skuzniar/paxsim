#ifndef OUCH_Modules_Factory_OUCH50_OrderFill_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"

#include "OUCH/OUCH50/OrderExecuted.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class OrderFill
{
public:
    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;
    using OrderExecuted   = OUCH::OUCH50::OrderExecuted;

    using AlgoIndicator         = OUCH::OUCH50::AlgoIndicator;
    using Market                = OUCH::OUCH50::Market;
    using TransactionCategory   = OUCH::OUCH50::TransactionCategory;
    using TradingMode           = OUCH::OUCH50::TradingMode;
    using Timestamp             = OUCH::OUCH50::Timestamp;
    using LiquidityAttributes   = OUCH::OUCH50::LiquidityAttributes;
    using LiquidityIndicator    = OUCH::OUCH50::LiquidityIndicator;
    using LiquidityInternalized = OUCH::OUCH50::LiquidityInternalized;
    using LiquidityTopOfBook    = OUCH::OUCH50::LiquidityTopOfBook;
    using LiquiditySelfTrade    = OUCH::OUCH50::LiquiditySelfTrade;
    using LiquidityFlag         = OUCH::OUCH50::LiquidityFlag;

    template<typename Context>
    explicit OrderFill(Context& context)
    {
    }

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
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
