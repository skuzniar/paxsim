#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Connector.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Modules/Factory/OUCH50/Parser.h"
#include "Modules/Factory/OUCH50/Session.h"
#include "Modules/Factory/OUCH50/Feeder.h"
#include "Modules/Factory/OUCH50/Writer.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Initiator.h"
#include "Modules/Feeder.h"
#include "Modules/Writer.h"

#include "OUCH50Client.h"

namespace OUCH::OUCH50 {

using namespace PaxSim;
using namespace PaxSim::Core;

void
execute(PaxSim::Core::IOContext& ioctx, const Config& config)
{
    // Context is an aggregate of context classes.
    using Context = Core::Aggregate<Context::Session>;

    Context context(config);

    using Parser    = OUCH::Modules::Parser<OUCH::Modules::Factory::OUCH50::Parser>;
    using Writer    = OUCH::Modules::Writer<OUCH::Modules::Factory::OUCH50::Writer>;
    using Session   = OUCH::Modules::Session<OUCH::Modules::Factory::OUCH50::Session>;
    using Initiator = OUCH::Modules::Initiator<OUCH::Modules::Factory::OUCH50::Session>;
    using Feeder    = OUCH::Modules::Feeder<OUCH::Modules::Factory::OUCH50::Feeder>;

    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator, Feeder>, Writer>;

    const auto& concfg = config["Session.Connector"];

    auto Connector = Core::Connector<Handler>(ioctx, concfg["Host"], concfg["Port"], concfg["Retries"], concfg["Delay"]);
    Connector.connect(config, context);
    ioctx.run();
}

} // namespace OUCH::OUCH50
