#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Connector.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "BOE2/Factory.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Initiator.h"
#include "Modules/Writer.h"

namespace BOE::BOE2 {

struct Client
{
    // Context is an aggregate of context classes. Not strictly needed here since we only have one context.
    using Context = PaxSim::Core::Aggregate<Context::Session>;

    using Parser    = BOE::Modules::Parser<BOE::BOE2::Factory>;
    using Writer    = BOE::Modules::Writer<BOE::BOE2::Factory>;
    using Session   = BOE::Modules::Session<BOE::BOE2::Factory>;
    using Initiator = BOE::Modules::Initiator<BOE::BOE2::Factory>;

    // Handler will be used once the connection to the server has been completed.
    using Handler = PaxSim::Core::Pipeline<Parser, PaxSim::Core::Flareout<Session, Initiator>, Writer>;
};

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext)
{
    // Create application context
    Client::Context context(config);

    // Expecting consistent connection configuration
    const auto& concfg = config["Session.Connector"];

    // Create connector that will activate client handler once the connection has been established
    PaxSim::Core::Connector<Client::Handler> connector(iocontext, concfg["Host"], concfg["Port"], concfg["Retries"], concfg["Delay"]);

    connector.connect(config, context);
    iocontext.run();
}

} // namespace BOE::BOE2
