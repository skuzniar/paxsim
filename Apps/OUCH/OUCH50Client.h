#ifndef OUCH_OUCH50Client_dot_h
#define OUCH_OUCH50Client_dot_h

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

namespace OUCH::OUCH50 {

using namespace PaxSim;

struct Client
{
    // Context is an aggregate of context classes. Not strictly needed here since we only have one context.
    using Context = Core::Aggregate<Context::Session>;

    using Parser    = OUCH::Modules::Parser<OUCH::Modules::Factory::OUCH50::Parser>;
    using Writer    = OUCH::Modules::Writer<OUCH::Modules::Factory::OUCH50::Writer>;
    using Session   = OUCH::Modules::Session<OUCH::Modules::Factory::OUCH50::Session>;
    using Initiator = OUCH::Modules::Initiator<OUCH::Modules::Factory::OUCH50::Session>;
    using Feeder    = OUCH::Modules::Feeder<OUCH::Modules::Factory::OUCH50::Feeder>;

    // Handler will be used once the connection to the server has been completed.
    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator, Feeder>, Writer>;
};

} // namespace OUCH::OUCH50

#endif
