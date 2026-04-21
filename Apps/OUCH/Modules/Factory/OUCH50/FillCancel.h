#ifndef OUCH_Modules_Factory_OUCH50_FillCancel_dot_h
#define OUCH_Modules_Factory_OUCH50_FillCancel_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Types.h"
#include "OUCH/OUCH50/TradeBroken.h"

#include "Common/Config.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace OUCH::OUCH50;
using namespace PaxSim::Core;
using Common::Config;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
struct FillCancel
{
    using TradeBroken = TradeBroken;

    template<typename Context>
    explicit FillCancel(Context& context)
    {
    }

    PacketHeader& cancel(const Fill& fill, const Order& order, const Config::Table& params)
    {
        auto& omsg = *new (m_buff) TradeBroken;

        omsg.timestamp           = Timestamp::now();
        omsg.userRefNum          = order.clordID();
        omsg.matchNumber         = fill.executionID();
        omsg.reason              = TradeBroken::enum_cast<TradeBroken::BreakReason>("Erroneous"); // TODO
        omsg.tradingMode         = TradingMode::ContinuousTrading;
        omsg.transactionCategory = TransactionCategory::NoneApply;
        omsg.algoIndicator       = AlgoIndicator::Algo;

        log << level::debug << vmark << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

private:
    char m_buff[1024];
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
