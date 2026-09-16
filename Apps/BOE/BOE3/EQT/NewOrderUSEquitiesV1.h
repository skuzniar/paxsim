#ifndef NewOrderUSEquitiesV1_h
#define NewOrderUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct NewOrderUSEquitiesV1
{
    enum : uint16_t
    {
        Type = 0x0fa1
    };

    Header header = { Type, sizeof(NewOrderUSEquitiesV1) };

    Text<20>            clOrdID;
    Side                side = Side::Buy;
    Binary<4>           orderQty;
    Alpha<4>            clearingFirm;
    Text<4>             clearingAccount;
    BinaryPrice         price;
    ExecInst            execInst;
    OrdType             ordType     = OrdType::Limit;
    TimeInForce         timeInForce = TimeInForce::Day;
    Binary<4>           minQty;
    Binary<4>           maxFloor;
    Alphanumeric<8>     symbol;
    Capacity            capacity = Capacity::Principal;
    RoutingInst         routingInst;
    Text<16>            account;
    DisplayIndicator    displayIndicator = DisplayIndicator::Default;
    Binary<2>           discretionAmount;
    BinaryPrice         pegDifference;
    PreventMatch        preventMatch;
    YesNo               locateRequired;
    DateTime            expireTime;
    AttributedQuote     attributedQuote;
    ExtExecInst         extExecInst;
    Binary<4>           displayRange;
    BinaryPrice         stopPrice;
    RoutStrategy        routStrategy;
    RouteDeliveryMethod routeDeliveryMethod;
    ExDestination       exDestination;
    Binary<2>           riskGroupID;
    CrossTradeFlag      crossTradeFlag = CrossTradeFlag::None;
    Text<4>             locateBroker;
    ShortBinaryPrice    stepUpAmount;
    Alpha<2>            cmcSessions;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
