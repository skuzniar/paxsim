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

#include "BOE2/Factory.h"

#include "Common/Params/OrderReject.h"
#include "Common/Params/OrderCancel.h"
#include "Common/Params/OrderFill.h"
#include "Common/Params/FillCancel.h"

#include "Common/Config.h"

namespace BOE::BOE2 {

struct Server
{
    // Params is an aggregate of parameter classes.
    using Params = PaxSim::Core::Aggregate<Common::Params::OrderReject, Common::Params::OrderFill, Common::Params::OrderCancel, Common::Params::FillCancel>;

    // Context is an aggregate of context classes.
    using Context = PaxSim::Core::Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    using Parser                  = BOE::Modules::Parser<BOE::BOE2::Factory>;
    using Session                 = BOE::Modules::Session<BOE::BOE2::Factory>;
    using OrderFlow               = BOE::Modules::OrderFlow<BOE::BOE2::Factory>;
    using OrderReject             = BOE::Modules::OrderReject<BOE::BOE2::Factory>;
    using OrderFill               = BOE::Modules::OrderFill<BOE::BOE2::Factory>;
    using OrderCancel             = BOE::Modules::OrderCancel<BOE::BOE2::Factory>;
    using OrderCancelOnDisconnect = BOE::Modules::OrderCancelOnDisconnect<BOE::BOE2::Factory>;
    using FillCancel              = BOE::Modules::FillCancel<BOE::BOE2::Factory>;
    using Writer                  = BOE::Modules::Writer<BOE::BOE2::Factory>;

    // Handler will be used once the session has been created.
    using Handler = PaxSim::Core::
        Pipeline<Parser, PaxSim::Core::Flareout<Session, OrderReject, OrderFlow, OrderCancelOnDisconnect, OrderCancel, FillCancel, OrderFill>, Writer>;
};

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext)
{
    // Expecting consistent connection configuration
    const auto& concfg = config["Session.Acceptor"];

    // Create application context
    Server::Context context(config);

    // Create acceptor that will activate server handler once the connection has been established
    PaxSim::Core::Acceptor<typename Server::Handler> acceptor(iocontext, concfg["Port"]);

    acceptor.listen(config, context);
    iocontext.run();
}

} // namespace BOE::BOE2
