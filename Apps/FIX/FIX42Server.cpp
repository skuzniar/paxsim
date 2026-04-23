#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
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

#include "FIX42Server.h"

namespace FIX::FIX42 {

using namespace Common;
using namespace PaxSim;
using namespace PaxSim::Core;

void
execute(PaxSim::Core::IOContext& ioctx, const Config& config)
{
    // Context is an aggregate of context classes.
    using Params  = Aggregate<Params::OrderReject, Params::OrderFill, Params::OrderCancel, Params::FillCancel, Params::FillCorrect>;
    using Context = Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    Context context(config);

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

    using Handler =
        Pipeline<Parser, Flareout<Session, OrderReject, OrderFlow, OrderFill, OrderCancel, OrderCancelOnDisconnect, FillCancel, FillCorrect>, Writer>;

    const auto& sescfg = config["Session"];

    if (!sescfg.valid()) {
        throw std::runtime_error("Invalid Session configuration");
    }

    const auto& saccfg = sescfg["Acceptor"];

    auto SAcceptor = Core::Acceptor<Handler>(ioctx, saccfg["Port"]);

    SAcceptor.listen(config, context);

    ioctx.run();
}

} // namespace FIX::FIX42
