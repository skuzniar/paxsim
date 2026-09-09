#ifndef OUCH_OUCH50Server_dot_h
#define OUCH_OUCH50Server_dot_h

#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"
#include "Context/OrderBook.h"
#include "Context/FillsBook.h"

#include "Modules/Factory/OUCH50/Parser.h"
#include "Modules/Factory/OUCH50/Session.h"
#include "Modules/Factory/OUCH50/OrderFlow.h"
#include "Modules/Factory/OUCH50/OrderReject.h"
#include "Modules/Factory/OUCH50/OrderFill.h"
#include "Modules/Factory/OUCH50/OrderCancel.h"
#include "Modules/Factory/OUCH50/OrderCancelOnDisconnect.h"
#include "Modules/Factory/OUCH50/FillCancel.h"
#include "Modules/Factory/OUCH50/Writer.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/OrderFlow.h"
#include "Modules/OrderReject.h"
#include "Modules/OrderFill.h"
#include "Modules/OrderCancel.h"
#include "Modules/OrderCancelOnDisconnect.h"
#include "Modules/FillCancel.h"
#include "Modules/Writer.h"

#include "Common/Params/OrderReject.h"
#include "Common/Params/OrderCancel.h"
#include "Common/Params/OrderFill.h"

#include "Common/Params/FillCancel.h"

namespace OUCH::OUCH50 {

using namespace PaxSim;

struct Server
{
    // Params is an aggregate of parameter classes.
    using Params = Core::Aggregate<Common::Params::OrderReject, Common::Params::OrderFill, Common::Params::OrderCancel, Common::Params::FillCancel>;

    // Context is an aggregate of context classes.
    using Context = Core::Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    using Parser                  = OUCH::Modules::Parser<OUCH::Modules::Factory::OUCH50::Parser>;
    using Session                 = OUCH::Modules::Session<OUCH::Modules::Factory::OUCH50::Session>;
    using OrderFlow               = OUCH::Modules::OrderFlow<OUCH::Modules::Factory::OUCH50::OrderFlow>;
    using OrderReject             = OUCH::Modules::OrderReject<OUCH::Modules::Factory::OUCH50::OrderReject>;
    using OrderFill               = OUCH::Modules::OrderFill<OUCH::Modules::Factory::OUCH50::OrderFill>;
    using OrderCancel             = OUCH::Modules::OrderCancel<OUCH::Modules::Factory::OUCH50::OrderCancel>;
    using OrderCancelOnDisconnect = OUCH::Modules::OrderCancelOnDisconnect<OUCH::Modules::Factory::OUCH50::OrderCancelOnDisconnect>;
    using FillCancel              = OUCH::Modules::FillCancel<OUCH::Modules::Factory::OUCH50::FillCancel>;
    using Writer                  = OUCH::Modules::Writer<OUCH::Modules::Factory::OUCH50::Writer>;

    // Handler will be used once the session has been created.
    using Handler =
        Core::Pipeline<Parser, Core::Flareout<Session, OrderReject, OrderFlow, OrderCancelOnDisconnect, OrderCancel, FillCancel, OrderFill>, Writer>;
};
} // namespace OUCH::OUCH50

#endif
