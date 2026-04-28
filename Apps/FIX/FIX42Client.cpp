#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Connector.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Modules/Factory/FIX42/Parser.h"
#include "Modules/Factory/FIX42/Session.h"
#include "Modules/Factory/FIX42/Feeder.h"
#include "Modules/Factory/FIX42/Writer.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Initiator.h"
#include "Modules/Feeder.h"
#include "Modules/Writer.h"

#include "FIX42Client.h"

namespace FIX::FIX42 {

using namespace PaxSim;
using namespace PaxSim::Core;

void
execute(PaxSim::Core::IOContext& ioctx, const Config& config)
{
    // Context is an aggregate of context classes.
    using Context = Core::Aggregate<Context::Session>;

    Context context(config);

    using Parser    = FIX::Modules::Parser<FIX::Modules::Factory::FIX42::Parser>;
    using Writer    = FIX::Modules::Writer<FIX::Modules::Factory::FIX42::Writer>;
    using Session   = FIX::Modules::Session<FIX::Modules::Factory::FIX42::Session>;
    using Initiator = FIX::Modules::Initiator<FIX::Modules::Factory::FIX42::Session>;
    using Feeder    = FIX::Modules::Feeder<FIX::Modules::Factory::FIX42::Feeder>;

    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator, Feeder>, Writer>;

    const auto& concfg = config["Session.Connector"];

    auto Connector = Core::Connector<Handler>(ioctx, concfg["Host"], concfg["Port"], concfg["Retries"], concfg["Delay"]);
    Connector.connect(config, context);
    ioctx.run();
}

} // namespace FIX::FIX42
