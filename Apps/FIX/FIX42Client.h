#ifndef FIX_FIX42Client_dot_h
#define FIX_FIX42Client_dot_h

#include "PaxSim/Core/IOContext.h"

#include "Common/Config.h"

namespace FIX::FIX42 {

using namespace Common;

void
execute(PaxSim::Core::IOContext& ioctx, const Config& config);

} // namespace FIX::FIX42

#endif
