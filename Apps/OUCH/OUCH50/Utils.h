#ifndef OUCH_OUCH50_Utils_h
#define OUCH_OUCH50_Utils_h

#include "AccountQuery.h"
#include "AccountQueryResponse.h"
#include "Appendage.h"
#include "CancelOrder.h"
#include "CancelPending.h"
#include "CancelRejected.h"
#include "ClientHeartbeat.h"
#include "EndOfSession.h"
#include "EnterOrder.h"
#include "LoginAccepted.h"
#include "LoginRejected.h"
#include "LoginRequest.h"
#include "LogoutRequest.h"
#include "OrderAccepted.h"
#include "OrderCancelled.h"
#include "OrderExecuted.h"
#include "OrderRejected.h"
#include "OrderReplaced.h"
#include "OrderRestated.h"
#include "PacketHeader.h"
#include "RefreshRequest.h"
#include "ReplaceOrder.h"
#include "ReplacePending.h"
#include "ServerHeartbeat.h"
#include "SystemEvent.h"
#include "TradeBroken.h"

namespace OUCH::OUCH50 {

// TODO - confess
struct PHeader : public PacketHeader
{};

struct SData : public SequencedData
{};

struct UHeader : public UnsequencedData
{};

inline std::ostream&
operator<<(std::ostream& s, const PHeader& o)
{
    s << PacketHeader(o);
    switch (o.type) {
        case SequencedData::Type:
            s << reinterpret_cast<const SequencedData&>(o);
            break;
        case UnsequencedData::Type:
            s << reinterpret_cast<const UnsequencedData&>(o);
            break;
        default:
            // log << level::info << in << '[' << msg << ']' << std::endl;
            break;
    }
    return s;
}

} // namespace OUCH::OUCH50

#endif
