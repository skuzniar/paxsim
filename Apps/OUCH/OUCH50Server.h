#ifndef OUCH_OUCH50Server_dot_h
#define OUCH_OUCH50Server_dot_h

#include "PaxSim/Core/IOContext.h"

#include "Common/Config.h"

namespace OUCH::OUCH50 {
void
execute(PaxSim::Core::IOContext& ioctx, const Common::Config& config);
} // namespace OUCH::OUCH50

#endif
