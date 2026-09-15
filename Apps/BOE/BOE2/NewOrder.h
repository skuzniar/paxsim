#ifndef NewOrder_h
#define NewOrder_h

#include "MessageHeader.h"
#include "Optional.h"

namespace boe::boe2 {

#pragma pack(1)
struct NewOrder
{
    static constexpr MessageType Type = MessageType::NewOrder;

    MessageHeader header = { Type, sizeof(NewOrder) };

    Text<20>        clOrdID;
    Alphanumeric<1> side;
    Binary<4>       orderQty;
    Binary<1>       numberOfNewOrderBitfields;

    using ClearingFirm    = OptionalField<1, 1, Optional::ClearingFirm>;
    using ClearingAccount = OptionalField<1, 2, Optional::ClearingAccount>;
    using Price           = OptionalField<1, 4, Optional::Price>;
    using ExecInst        = OptionalField<1, 8, Optional::ExecInst>;
    using OrdType         = OptionalField<1, 16, Optional::OrdType>;
    using TimeInForce     = OptionalField<1, 32, Optional::TimeInForce>;
    using MinQty          = OptionalField<1, 64, Optional::MinQty>;
    using MaxFloor        = OptionalField<1, 128, Optional::MaxFloor>;
    using Symbol          = OptionalField<2, 1, Optional::Symbol>;
    using SymbolSfx       = OptionalField<2, 2, Optional::SymbolSfx>;
    // using Currency              = OptionalField<2, 4, Optional::Currency>;
    // using IdSource              = OptionalField<2, 8, Optional::IdSource>;
    // using SecurityId            = OptionalField<2, 16, Optional::SecurityId>;
    // using SecurityExchange      = OptionalField<2, 32, Optional::SecurityExchange>;
    using Capacity         = OptionalField<2, 64, Optional::Capacity>;
    using RoutingInst      = OptionalField<2, 128, Optional::RoutingInst>;
    using Account          = OptionalField<3, 1, Optional::Account>;
    using DisplayIndicator = OptionalField<3, 2, Optional::DisplayIndicator>;
    using DiscretionAmount = OptionalField<3, 8, Optional::DiscretionAmount>;
    using PegDifference    = OptionalField<3, 16, Optional::PegDifference>;
    using PreventMatch     = OptionalField<3, 32, Optional::PreventMatch>;
    using LocateReqd       = OptionalField<3, 64, Optional::LocateReqd>;
    using ExpireTime       = OptionalField<3, 128, Optional::ExpireTime>;
    // using MaturityDate          = OptionalField<4, 1, Optional::MaturityDate>;
    // using StrikePrice           = OptionalField<4, 2, Optional::StrikePrice>;
    // using PutOrCall             = OptionalField<4, 4, Optional::PutOrCall>;
    using RiskReset           = OptionalField<4, 8, Optional::RiskReset>;
    using DisplayRange        = OptionalField<6, 1, Optional::DisplayRange>;
    using StopPx              = OptionalField<6, 2, Optional::StopPx>;
    using RoutStrategy        = OptionalField<6, 4, Optional::RoutStrategy>;
    using RouteDeliveryMethod = OptionalField<6, 8, Optional::RouteDeliveryMethod>;
    using ExDestination       = OptionalField<6, 16, Optional::ExDestination>;
    using EchoText            = OptionalField<6, 32, Optional::EchoText>;
    // using AuctionId             = OptionalField<6, 64, Optional::AuctionId>;
    // using RoutingFirmID         = OptionalField<6, 128, Optional::RoutingFirmID>;
    // using AlgorithmicIndicator  = OptionalField<7, 1, Optional::AlgorithmicIndicator>;
    using RiskGroupID = OptionalField<7, 2, Optional::RiskGroupID>;
    // using ClientQualifiedRole   = OptionalField<7, 4, Optional::ClientQualifiedRole>;
    // using InvestorQualifiedRole = OptionalField<7, 8, Optional::InvestorQualifiedRole>;
    // using ExecutorQualifiedRole = OptionalField<7, 16, Optional::ExecutorQualifiedRole>;
    // using CtiCode               = OptionalField<7, 32, Optional::CtiCode>;
    // using ManualOrderIndicator  = OptionalField<7, 64, Optional::ManualOrderIndicator>;
    // using OperatorId            = OptionalField<7, 128, Optional::OperatorId>;
    // using ClearingOptionalData  = OptionalField<8, 4, Optional::ClearingOptionalData>;
    // using ClientIDAttr          = OptionalField<8, 8, Optional::ClientIDAttr>;
    // using FrequentTraderID      = OptionalField<8, 16, Optional::FrequentTraderID>;
    // using Compression           = OptionalField<8, 32, Optional::Compression>;
    // using FloorDestination      = OptionalField<8, 64, Optional::FloorDestination>;
    // using FloorRoutingInst      = OptionalField<8, 128, Optional::FloorRoutingInst>;
    // using OrderOrigin           = OptionalField<9, 1, Optional::OrderOrigin>;
    // using ORS                   = OptionalField<9, 2, Optional::ORS>;
    // using PriceType             = OptionalField<9, 4, Optional::PriceType>;
};
#pragma pack()

} // namespace boe::boe2

#endif
