#ifndef ModifyOrder_h
#define ModifyOrder_h

#include "MessageHeader.h"

namespace boe::boe2 {

#pragma pack(1)
struct ModifyOrder
{
    static constexpr MessageType Type = MessageType::ModifyOrder;

    MessageHeader header = { Type, sizeof(ModifyOrder) };

    Text<20>  clOrdID;
    Text<20>  origClOrdID;
    Binary<1> numberOfModifyOrderBitfields;
};
#pragma pack()

} // namespace boe::boe2

#endif
