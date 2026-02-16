#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Connector.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"
#include "PaxSim/Core/Context.h"

#include "Factory/Session/FIX42Session.h"
#include "Factory/Writer/FIX42Writer.h"

#include "Modules/Parser/Parser.h"
#include "Modules/Session/Session.h"
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
    using Context = Core::Context<Fix::Context::Session>;

    // Configuration object will be passed to each context component.
    Context ctx(cfg);

    using Parser  = Fix::Modules::Parser::Parser;
    using Writer  = Fix::Modules::Writer::Writer<Fix::Factory::Writer::FIX42Writer>;
    using Session = Fix::Modules::Session::Session<Fix::Factory::Session::FIX42Session>;

    using Handler = Core::Pipeline<Parser, Core::Flareout<Session>, Writer>;

    const auto& concfg = cfg["Session.Connector"];

    auto Connector = Core::Connector<Handler, Context>(context, ctx, concfg["Host"], concfg["Port"], concfg["Retries"], concfg["Delay"]);
    Connector.connect();
    context.run();
}

} // namespace Fix::Fix42
