#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"
#include "Context/OrderBook.h"
#include "Context/FillsBook.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/OrderFlow.h"
#include "Modules/OrderReject.h"
#include "Modules/OrderFill.h"
#include "Modules/OrderCancel.h"
#include "Modules/OrderCancelOnDisconnect.h"
#include "Modules/FillCancel.h"
#include "Modules/Writer.h"

#include "Common/Control/Parser.h"
#include "Common/Control/Admin.h"
#include "Common/Control/Loader.h"
#include "Common/Control/Writer.h"

#include "OUCH50/Factory.h"

#include "Common/Params/OrderReject.h"
#include "Common/Params/OrderCancel.h"
#include "Common/Params/OrderFill.h"
#include "Common/Params/FillCancel.h"
#include "Common/Params/Store.h"

#include "Common/Config.h"

namespace OUCH::OUCH50 {

struct Server
{
    // Params is an aggregate of parameter classes.
    using Params = Common::Params::Store<Common::Params::OrderReject, Common::Params::OrderFill, Common::Params::OrderCancel, Common::Params::FillCancel>;

    // Context is an aggregate of context classes.
    using Context = PaxSim::Core::Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    using Parser                  = OUCH::Modules::Parser<OUCH::OUCH50::Factory>;
    using Session                 = OUCH::Modules::Session<OUCH::OUCH50::Factory>;
    using OrderFlow               = OUCH::Modules::OrderFlow<OUCH::OUCH50::Factory>;
    using OrderReject             = OUCH::Modules::OrderReject<OUCH::OUCH50::Factory>;
    using OrderFill               = OUCH::Modules::OrderFill<OUCH::OUCH50::Factory>;
    using OrderCancel             = OUCH::Modules::OrderCancel<OUCH::OUCH50::Factory>;
    using OrderCancelOnDisconnect = OUCH::Modules::OrderCancelOnDisconnect<OUCH::OUCH50::Factory>;
    using FillCancel              = OUCH::Modules::FillCancel<OUCH::OUCH50::Factory>;
    using Writer                  = OUCH::Modules::Writer<OUCH::OUCH50::Factory>;

    // Handler defines message processing pipeline
    using Handler = PaxSim::Core::
        Pipeline<Parser, PaxSim::Core::Flareout<Session, OrderReject, OrderFlow, OrderCancelOnDisconnect, OrderCancel, FillCancel, OrderFill>, Writer>;

    using CtlParser = Common::Control::Parser;
    using CtlAdmin  = Common::Control::Admin<Params>;
    using CtlLoader = Common::Control::Loader<Params>;
    using CtlWriter = Common::Control::Writer;

    // Controller handles control commands
    using Controller = PaxSim::Core::Pipeline<CtlParser, PaxSim::Core::Flareout<CtlAdmin, CtlLoader>, CtlWriter>;
};

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext)
{
    // Create application context
    Server::Context context(config);

    // Create application acceptor that will activate server handler once the connection has been established
    PaxSim::Core::Acceptor<typename Server::Handler> appacceptor(iocontext, static_cast<int>(config["Session.Acceptor.Port"]));
    appacceptor.listen(config, context);

    // Create controller acceptor that will activate control handler once the connection has been established
    PaxSim::Core::Acceptor<typename Server::Controller> ctlacceptor(iocontext, static_cast<int>(config["Control.Acceptor.Port"]));
    ctlacceptor.listen(config, context);

    iocontext.run();
}

} // namespace OUCH::OUCH50
