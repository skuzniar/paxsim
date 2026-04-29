#ifndef OUCH_Modules_Factory_OUCH50_OrderFill_dot_h
#define OUCH_Modules_Factory_OUCH50_OrderFill_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"

#include "OUCH/OUCH50/OrderExecuted.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace OUCH::OUCH50;

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class OrderFill
{
public:
    using PacketHeader    = PacketHeader;
    using SequencedData   = SequencedData;
    using UnsequencedData = UnsequencedData;

    using OrderExecute = OrderExecuted;

    template<typename Context>
    explicit OrderFill(Context& context)
    {
    }

    PacketHeader& report(const Fill& fill, const Order& order)
    {
        auto& omsg = *new (m_buff) OrderExecuted;

        omsg.timestamp           = Timestamp::now();
        omsg.userRefNum          = order.clordID();
        omsg.executedQuantity    = fill.quantity();
        omsg.executionPrice      = fill.price();
        omsg.liquidityFlag       = LiquidityFlag::AuctionTrade;
        omsg.matchNumber         = fill.executionID();
        omsg.contraFirm          = std::string_view("PaxFrm");
        omsg.tradingMode         = TradingMode::ContinuousTrading;
        omsg.transactionCategory = TransactionCategory::NoneApply;
        omsg.algoIndicator       = AlgoIndicator::Algo;
        omsg.liquidityAttributes = LiquidityAttributes();
        omsg.lastMarket          = Market::DHEL;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

private:
    char m_buff[1024];
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
