#ifndef FIX_FIX42Client_dot_h
#define FIX_FIX42Client_dot_h

#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Flareout.h"

#include "Context/Session.h"
#include "Context/FillsBook.h"

#include "Modules/Parser.h"
#include "Modules/Session.h"
#include "Modules/Initiator.h"
#include "Modules/Writer.h"

#include "FIX/FIX42/Factory.h"

namespace FIX::FIX42 {

using namespace PaxSim;
using namespace PaxSim::Core;

struct Client
{
    // Context is an aggregate of context classes. Not strictly needed here since we only have one context.
    using Context = Core::Aggregate<Context::Session, Context::FillsBook>;

    using Parser    = FIX::Modules::Parser<FIX::FIX42::Factory>;
    using Writer    = FIX::Modules::Writer<FIX::FIX42::Factory>;
    using Session   = FIX::Modules::Session<FIX::FIX42::Factory>;
    using Initiator = FIX::Modules::Initiator<FIX::FIX42::Factory>;

    // Handler will be used once the connection to the server has been completed.
    using Handler = Core::Pipeline<Parser, Core::Flareout<Session, Initiator>, Writer>;
};

} // namespace FIX::FIX42

#endif
