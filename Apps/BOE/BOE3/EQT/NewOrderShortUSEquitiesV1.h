#ifndef NewOrderShortUSEquitiesV1_h
#define NewOrderShortUSEquitiesV1_h

#include "Header.h"

namespace boe::boe3::eqt {

#pragma pack(1)
struct NewOrderShortUSEquitiesV1
{
    enum
    {
        Type = 0x0fa6
    };

    Header header = { Type, sizeof(NewOrderShortUSEquitiesV1) };

    Text<20>         clOrdID;
    Side             side;
    Binary<4>        orderQty;
    Alpha<4>         clearingFirm;
    Text<4>          clearingAccount;
    BinaryPrice      price;
    ExecInst         execInst;
    OrdType          ordType;
    TimeInForce      timeInForce;
    Alphanumeric<8>  symbol;
    Capacity         capacity;
    RoutingInst      routingInstructions;
    Text<10>         shortAccount;
    DisplayIndicator displayIndicator = DisplayIndicator::Default;
    Binary<2>        riskGroupID;
    Text<4>          locateBroker;
};
#pragma pack()

} // namespace boe::boe3::eqt

#endif
