#ifndef OUCH_OUCH50Client_dot_h
#define OUCH_OUCH50Client_dot_h

#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "OUCH50/Factory.h"

#include "Context/Session.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Initiator.h"
#include "Modules/Writer.h"

namespace OUCH::OUCH50 {

using namespace PaxSim;

struct Client
{
    // Context is an aggregate of context classes. Not strictly needed here since we only have one context.
    using Context = Core::Aggregate<Context::Session>;

    using Parser    = OUCH::Modules::Parser<OUCH::OUCH50::Factory>;
    using Writer    = OUCH::Modules::Writer<OUCH::OUCH50::Factory>;
    using Session   = OUCH::Modules::Session<OUCH::OUCH50::Factory>;
    using Initiator = OUCH::Modules::Initiator<OUCH::OUCH50::Factory>;

    // Handler will be used once the connection to the server has been completed.
    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator>, Writer>;
};

} // namespace OUCH::OUCH50

#endif
