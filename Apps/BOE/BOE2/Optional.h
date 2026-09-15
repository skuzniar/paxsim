#ifndef Optional_h
#define Optional_h

#include "Types.h"

namespace boe::boe2::Optional {
using Account                = Text<16>;
using AttributedQuote        = Alphanumeric<1>;
using BaseLiquidityIndicator = Alphanumeric<1>;
using CancelOrigOnReject     = Alpha<1>;
using Capacity               = Alpha<1>;
using ClearingAccount        = Text<4>;
using ClearingFirm           = Alpha<4>;
using CmcMatchQty            = Binary<4>;
using CmcSessions            = Text<2>;
using CrossTradeFlag         = Alphanumeric<1>;
using DiscretionAmount       = Binary<2>;
using DisplayIndicator       = Alphanumeric<1>;
using DisplayPrice           = BinaryPrice;
using DisplayRange           = Binary<4>;
using EchoText               = Text<64>;
using ExDestination          = Text<1>;
using ExecInst               = Text<1>;
using ExpireTime             = DateTime;
using ExtExecInst            = Text<1>;
using FeeCode                = Alphanumeric<2>;
using LastPx                 = BinaryPrice;
using LastShares             = Binary<4>;
using LeavesQty              = Binary<4>;
using LocateBroker           = Alpha<4>;
using LocateReqd             = Alpha<1>;
using MassCancelID           = Text<20>;
using MassCancelInst         = Text<16>;
using MatchingUnit           = Binary<1>;
using MaxFloor               = Binary<4>;
using MinQty                 = Binary<4>;
using OrderQty               = Binary<4>;
using OrdType                = Alphanumeric<1>;
using OrigClOrdID            = Text<20>;
using PegDifference          = BinaryPrice;
using PreventMatch           = Alpha<3>;
using Price                  = BinaryPrice;
using RiskGroupID            = Binary<2>;
using RiskReset              = Text<8>;
using RouteDeliveryMethod    = Text<3>;
using RoutingInst            = Text<4>;
using RoutStrategy           = Text<6>;
using SecondaryOrderID       = Binary<8>;
using Side                   = Alphanumeric<1>;
using StopPx                 = BinaryPrice;
using StepUpAmount           = BinaryPrice;
using SubLiquidityIndicator  = Alphanumeric<1>;
using Symbol                 = Alphanumeric<8>;
using SymbolSfx              = Alphanumeric<8>;
using TimeInForce            = Alphanumeric<1>;
using WorkingPrice           = BinaryPrice;
} // namespace boe::boe2::Optional

#endif
