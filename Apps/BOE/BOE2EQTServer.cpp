#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Writer.h"

#include "BOE2/Factory.h"

#include "Common/Config.h"

namespace boe::boe2 {

struct Server
{
    // Context is an aggregate of context classes.
    using Context = PaxSim::Core::Aggregate<Context::Session>;

    using Parser  = boe::Modules::Parser<boe::boe2::Factory>;
    using Session = boe::Modules::Session<boe::boe2::Factory>;
    using Writer  = boe::Modules::Writer<boe::boe2::Factory>;

    // Handler will be used once the session has been created.
    using Handler = PaxSim::Core::Pipeline<Parser, PaxSim::Core::Flareout<Session>, Writer>;
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

} // namespace boe::boe2
