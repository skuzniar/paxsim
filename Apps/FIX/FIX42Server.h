#ifndef FIX_FIX42Server_dot_h
#define FIX_FIX42Server_dot_h

#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"
#include "Context/OrderBook.h"
#include "Context/FillsBook.h"

#include "Modules/Factory/FIX42/Parser.h"
#include "Modules/Factory/FIX42/Session.h"
#include "Modules/Factory/FIX42/OrderFlow.h"
#include "Modules/Factory/FIX42/OrderReject.h"
#include "Modules/Factory/FIX42/OrderFill.h"
#include "Modules/Factory/FIX42/OrderCancel.h"
#include "Modules/Factory/FIX42/OrderCancelOnDisconnect.h"
#include "Modules/Factory/FIX42/FillCancel.h"
#include "Modules/Factory/FIX42/FillCorrect.h"
#include "Modules/Factory/FIX42/Writer.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/OrderFlow.h"
#include "Modules/OrderReject.h"
#include "Modules/OrderFill.h"
#include "Modules/OrderCancel.h"
#include "Modules/OrderCancelOnDisconnect.h"
#include "Modules/FillCancel.h"
#include "Modules/FillCorrect.h"
#include "Modules/Writer.h"

#include "Common/Params/OrderReject.h"
#include "Common/Params/OrderCancel.h"
#include "Common/Params/OrderFill.h"

#include "Common/Params/FillCancel.h"
#include "Common/Params/FillCorrect.h"

namespace FIX::FIX42 {

using namespace PaxSim;
using namespace PaxSim::Core;
using namespace Common;

struct Server
{
    // Params is an aggregate of parameter classes.
    using Params = Aggregate<Params::OrderReject, Params::OrderFill, Params::OrderCancel, Params::FillCancel, Params::FillCorrect>;

    // Context is an aggregate of context classes.
    using Context = Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    using Parser                  = FIX::Modules::Parser<FIX::Modules::Factory::FIX42::Parser>;
    using Writer                  = FIX::Modules::Writer<FIX::Modules::Factory::FIX42::Writer>;
    using Session                 = FIX::Modules::Session<FIX::Modules::Factory::FIX42::Session>;
    using OrderFlow               = FIX::Modules::OrderFlow<FIX::Modules::Factory::FIX42::OrderFlow>;
    using OrderReject             = FIX::Modules::OrderReject<FIX::Modules::Factory::FIX42::OrderReject>;
    using OrderFill               = FIX::Modules::OrderFill<FIX::Modules::Factory::FIX42::OrderFill>;
    using OrderCancel             = FIX::Modules::OrderCancel<FIX::Modules::Factory::FIX42::OrderCancel>;
    using OrderCancelOnDisconnect = FIX::Modules::OrderCancelOnDisconnect<FIX::Modules::Factory::FIX42::OrderCancelOnDisconnect>;
    using FillCancel              = FIX::Modules::FillCancel<FIX::Modules::Factory::FIX42::FillCancel>;
    using FillCorrect             = FIX::Modules::FillCorrect<FIX::Modules::Factory::FIX42::FillCorrect>;

    // Handler will be used once the session has been created.
    using Handler =
        Pipeline<Parser, Flareout<Session, OrderReject, OrderFlow, OrderCancelOnDisconnect, OrderCancel, FillCancel, FillCorrect, OrderFill>, Writer>;
};
} // namespace FIX::FIX42

#endif
