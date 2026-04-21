#ifndef Appendage_h
#define Appendage_h

#include "PacketHeader.h"

namespace OUCH::OUCH50::appendage {

#pragma pack(1)
using ClearingAccount               = Appendage<1, Alpha<12>>;
using ClearingAccountType           = Appendage<2, ClearingAccountType>;
using ClearingFirm                  = Appendage<3, Alpha<4>>;
using ClientReference               = Appendage<4, Alpha<15>>;
using CrossType                     = Appendage<5, CrossType>;
using DEAIndicator                  = Appendage<6, DEAIndicator>;
using Display                       = Appendage<7, Display>;
using DisplayPrice                  = Appendage<8, Price>;
using DisplayQuantity               = Appendage<9, Int32>;
using ExpireTime                    = Appendage<10, Int16>;
using Firm                          = Appendage<11, Alpha<4>>;
using LiquidityProvisionIndicator   = Appendage<12, LiquidityProvisionIndicator>;
using MaxFloor                      = Appendage<13, Int32>;
using MinimumQuantity               = Appendage<14, Int32>;
using OrderReference                = Appendage<15, Alpha<10>>;
using OriginalOrderEntryDate        = Appendage<16, Int32>;
using OriginalOrderReferenceNumber  = Appendage<17, Int64>;
using PegDifference                 = Appendage<18, Int32>;
using PegType                       = Appendage<19, PegType>;
using RandomReserve                 = Appendage<20, Int32>;
using SecondaryOrderReferenceNumber = Appendage<21, Int64>;
using STPAction                     = Appendage<22, STPAction>;
using STPLevel                      = Appendage<23, STPLevel>;
using STPTraderGroup                = Appendage<24, Alpha<2>>;
using TimeInForce                   = Appendage<25, TimeInForce>;
using TradingAtClosing              = Appendage<26, Alpha<1>>;
using OrderCondition                = Appendage<27, OrderCondition>;
using CumulativeQuantity            = Appendage<28, Int32>;
using CustomerOrderCapacity         = Appendage<29, Alpha<1>>;
#pragma pack()

} // namespace OUCH::OUCH50::appendage

#endif
