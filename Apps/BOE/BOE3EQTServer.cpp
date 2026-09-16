#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Writer.h"

#include "BOE3/EQT/Factory.h"

#include "Common/Config.h"

namespace boe::boe3::eqt {

struct Server
{
    // Context is an aggregate of context classes.
    using Context = PaxSim::Core::Aggregate<Context::Session>;

    using Parser  = boe::Modules::Parser<boe::boe3::eqt::Factory>;
    using Session = boe::Modules::Session<boe::boe3::eqt::Factory>;
    using Writer  = boe::Modules::Writer<boe::boe3::eqt::Factory>;

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

} // namespace boe::boe3::eqt
