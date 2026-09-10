#ifndef FIX_FIX42Client_dot_h
#define FIX_FIX42Client_dot_h

#include "PaxSim/Core/IOContext.h"
#include "Common/Config.h"

namespace FIX::FIX42 {

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext);

} // namespace FIX::FIX42

#endif
