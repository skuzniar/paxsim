#ifndef OUCH_OUCH50Client_dot_h
#define OUCH_OUCH50Client_dot_h

#include "PaxSim/Core/IOContext.h"
#include "Common/Config.h"

namespace OUCH::OUCH50 {

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext);

} // namespace OUCH::OUCH50

#endif
