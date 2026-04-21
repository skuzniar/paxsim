#ifndef OUCH_OUCH50Client_dot_h
#define OUCH_OUCH50Client_dot_h

#include "PaxSim/Core/IOContext.h"

#include "Common/Config.h"

namespace OUCH::OUCH50 {

using namespace Common;

void
execute(PaxSim::Core::IOContext& ioctx, const Config& config);

} // namespace OUCH::OUCH50

#endif
