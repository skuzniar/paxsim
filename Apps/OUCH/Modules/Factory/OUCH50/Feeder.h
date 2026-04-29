#ifndef OUCH_Modules_Factory_OUCH50_Feeder_dot_h
#define OUCH_Modules_Factory_OUCH50_Feeder_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "OUCH/Context/Session.h"
#include "OUCH/Types.h"

#include "OUCH/OUCH50/EnterOrder.h"
#include "OUCH/OUCH50/ReplaceOrder.h"
#include "OUCH/OUCH50/CancelOrder.h"

namespace OUCH::Modules::Factory::OUCH50 {

using namespace OUCH::OUCH50;
using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory.
//---------------------------------------------------------------------------------------------------------------------
class Feeder
{
    using PacketHeader = PacketHeader;

    using EnterOrder   = EnterOrder;
    using ReplaceOrder = ReplaceOrder;
    using CancelOrder  = CancelOrder;

public:
    template<typename Context>
    explicit Feeder(Context& context)
      : m_session(context)
    {
    }

    PacketHeader& enterOrder()
    {
        auto& omsg = *new (m_buff) EnterOrder;

        omsg.userRefNum                   = m_session.oSequenceAdvance();
        omsg.buySellIndicator             = BuySellIndicator::Buy;
        omsg.quantity                     = 500;
        omsg.orderBook                    = 1111;
        omsg.price                        = 4.99;
        omsg.user                         = std::string_view("PaxSim");
        omsg.executionWithinFirm          = 4004;
        omsg.investmentDecisionWithinFirm = 4005;
        omsg.clientIdentifier             = 4006;
        omsg.partyRoleQualifier           = { ClientIdentification::Firm, InvestmentDecisionMaker::Algo, ExecutionDecisionWithinFirm::Algo };
        omsg.capacity                     = Capacity::Client;
        omsg.algoIndicator                = AlgoIndicator::NoAlgo;

        m_clordid = omsg.userRefNum;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& enterReject()
    {
        auto& omsg = *new (m_buff) EnterOrder;

        omsg.userRefNum                   = m_session.oSequenceAdvance();
        omsg.buySellIndicator             = BuySellIndicator::Buy;
        omsg.quantity                     = 5101;
        omsg.orderBook                    = 1111;
        omsg.price                        = 4.99;
        omsg.user                         = std::string_view("PaxSim");
        omsg.executionWithinFirm          = 4004;
        omsg.investmentDecisionWithinFirm = 4005;
        omsg.clientIdentifier             = 4006;
        omsg.partyRoleQualifier           = { ClientIdentification::Firm, InvestmentDecisionMaker::Algo, ExecutionDecisionWithinFirm::Algo };
        omsg.capacity                     = Capacity::Client;
        omsg.algoIndicator                = AlgoIndicator::NoAlgo;

        m_clordid = omsg.userRefNum;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& replaceOrder()
    {
        auto& omsg = *new (m_buff) ReplaceOrder;

        omsg.origUserRefNum = m_clordid;
        omsg.newUserRefNum  = m_session.oSequenceAdvance();
        omsg.quantity       = 600;
        omsg.price          = 5.99;
        omsg.user           = std::string_view("PaxSim");

        m_clordid = omsg.newUserRefNum;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& replaceReject()
    {
        auto& omsg = *new (m_buff) ReplaceOrder;

        omsg.origUserRefNum = m_clordid;
        omsg.newUserRefNum  = m_session.oSequenceAdvance();
        omsg.quantity       = 5102;
        omsg.price          = 5.99;
        omsg.user           = std::string_view("PaxSim");

        m_clordid = omsg.newUserRefNum;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& cancelOrder()
    {
        auto& omsg = *new (m_buff) CancelOrder;

        omsg.userRefNum = m_clordid;
        omsg.quantity   = 600;
        omsg.user       = std::string_view("PaxSim");

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

    PacketHeader& orderFill()
    {
        auto& omsg = *new (m_buff) EnterOrder;

        omsg.userRefNum                   = m_session.oSequenceAdvance();
        omsg.buySellIndicator             = BuySellIndicator::Buy;
        omsg.quantity                     = 6104;
        omsg.orderBook                    = 1111;
        omsg.price                        = 4.99;
        omsg.user                         = std::string_view("PaxSim");
        omsg.executionWithinFirm          = 4004;
        omsg.investmentDecisionWithinFirm = 4005;
        omsg.clientIdentifier             = 4006;
        omsg.partyRoleQualifier           = { ClientIdentification::Firm, InvestmentDecisionMaker::Algo, ExecutionDecisionWithinFirm::Algo };
        omsg.capacity                     = Capacity::Client;
        omsg.algoIndicator                = AlgoIndicator::NoAlgo;

        m_clordid = omsg.userRefNum;

        log << level::debug << oflow << '[' << omsg << ']' << std::endl;

        return reinterpret_cast<PacketHeader&>(omsg);
    }

private:
    Context::Session& m_session;
    char              m_buff[1024];

    // Keep some details across calls
    uint32_t m_clordid = 0;
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
