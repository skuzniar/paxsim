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
#include "Modules/FillCorrect.h"
#include "Modules/Writer.h"

#include "FIX42/Factory.h"

#include "Params/OrderReject.h"
#include "Params/OrderCancel.h"
#include "Params/OrderFill.h"

#include "Params/FillCancel.h"
#include "Params/FillCorrect.h"

namespace FIX::FIX42 {

struct Server
{
    // Params is an aggregate of parameter classes.
    using Params = Aggregate<Params::OrderReject, Params::OrderFill, Params::OrderCancel, Params::FillCancel, Params::FillCorrect>;

    // Context is an aggregate of context classes.
    using Context = Aggregate<Context::Session, Context::OrderBook, Context::FillsBook, Params>;

    using Parser                  = FIX::Modules::Parser<FIX::FIX42::Factory>;
    using Writer                  = FIX::Modules::Writer<FIX::FIX42::Factory>;
    using Session                 = FIX::Modules::Session<FIX::FIX42::Factory>;
    using OrderFlow               = FIX::Modules::OrderFlow<FIX::FIX42::Factory>;
    using OrderReject             = FIX::Modules::OrderReject<FIX::FIX42::Factory>;
    using OrderFill               = FIX::Modules::OrderFill<FIX::FIX42::Factory>;
    using OrderCancel             = FIX::Modules::OrderCancel<FIX::FIX42::Factory>;
    using OrderCancelOnDisconnect = FIX::Modules::OrderCancelOnDisconnect<FIX::FIX42::Factory>;
    using FillCancel              = FIX::Modules::FillCancel<FIX::FIX42::Factory>;
    using FillCorrect             = FIX::Modules::FillCorrect<FIX::FIX42::Factory>;

    // Handler will be used once the session has been created.
    using Handler =
        Pipeline<Parser, Flareout<Session, OrderReject, OrderFlow, OrderCancelOnDisconnect, OrderCancel, FillCancel, FillCorrect, OrderFill>, Writer>;
};

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext)
{
    // Create application context
    Server::Context context(config);

    // Create acceptor that will activate server handler once the connection has been established
    PaxSim::Core::Acceptor<typename Server::Handler> acceptor(iocontext, static_cast<int>(config["Session.Acceptor.Port"]));

    acceptor.listen(config, context);
    iocontext.run();
}

} // namespace FIX::FIX42
