#ifndef FIX_FIX42Client_dot_h
#define FIX_FIX42Client_dot_h

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

namespace FIX::FIX42 {

using namespace PaxSim;
using namespace PaxSim::Core;

struct Client
{
    // Context is an aggregate of context classes. Not strictly needed here since we only have one context.
    using Context = Core::Aggregate<Context::Session>;

    using Parser    = FIX::Modules::Parser<FIX::Modules::Factory::FIX42::Parser>;
    using Writer    = FIX::Modules::Writer<FIX::Modules::Factory::FIX42::Writer>;
    using Session   = FIX::Modules::Session<FIX::Modules::Factory::FIX42::Session>;
    using Initiator = FIX::Modules::Initiator<FIX::Modules::Factory::FIX42::Session>;
    using Feeder    = FIX::Modules::Feeder<FIX::Modules::Factory::FIX42::Feeder>;

    // Handler will be used once the connection to the server has been completed.
    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator, Feeder>, Writer>;
};

} // namespace FIX::FIX42

#endif
