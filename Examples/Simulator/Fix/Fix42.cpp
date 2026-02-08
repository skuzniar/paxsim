#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Context.h"

#include "Factory/Session/FIX42Session.h"
#include "Factory/OrderFlow/FIX42Inflow.h"
#include "Factory/OrderFlow/FIX42Reject.h"
#include "Factory/OrderBook/FIX42Executions.h"
#include "Factory/ExecutionBook/FIX42Cancellations.h"
#include "Factory/ExecutionBook/FIX42Corrections.h"
#include "Factory/Writer/FIX42Writer.h"

#include "Modules/Parser/Parser.h"
#include "Modules/Session/Session.h"
#include "Modules/OrderFlow/Inflow.h"
#include "Modules/OrderFlow/Reject.h"
#include "Modules/OrderBook/Executions.h"
#include "Modules/ExecutionBook/Cancellations.h"
#include "Modules/ExecutionBook/Corrections.h"
#include "Modules/Writer/Writer.h"

#include "Fix42.h"

namespace Fix::Fix42 {

using namespace PaxSim;
using namespace PaxSim::Core;

void
simulate(const Config& cfg)
{
    Core::IOContext context;

    // Context is an aggregate of context classes.
    using Context = Core::Context<Fix::Context::Session, Fix::Context::OrderBook, Fix::Context::ExecutionBook>;

    // Configuration object will be passed to each context component.
    Context ctx(cfg);

    using Writer        = Fix::Modules::Writer::Writer<Fix::Factory::Writer::FIX42Writer>;
    using Session       = Fix::Modules::Session::Session<Fix::Factory::Session::FIX42Session>;
    using Inflow        = Fix::Modules::OrderFlow::Inflow<Fix::Factory::OrderFlow::FIX42Inflow>;
    using Reject        = Fix::Modules::OrderFlow::Reject<Fix::Factory::OrderFlow::FIX42Reject>;
    using Executions    = Fix::Modules::OrderBook::Executions<Fix::Factory::OrderBook::FIX42Executions>;
    using Cancellations = Fix::Modules::ExecutionBook::Cancellations<Fix::Factory::ExecutionBook::FIX42Cancellations>;
    using Corrections   = Fix::Modules::ExecutionBook::Corrections<Fix::Factory::ExecutionBook::FIX42Corrections>;

    using Handler = Core::Pipeline<Fix::Modules::Parser::Parser, Core::Flareout<Session, Reject, Inflow, Executions, Cancellations, Corrections>, Writer>;

    const auto& acccfg = cfg["Session.Acceptor"];

    auto Acceptor = Core::Acceptor<Handler, Context>(ctx, acccfg["Port"], context);
    Acceptor.listen();
    context.run();
}

} // namespace Fix::Fix42
