#ifndef BOE_BOE2Client_dot_h
#define BOE_BOE2Client_dot_h

#include "PaxSim/Core/IOContext.h"
#include "Common/Config.h"

namespace boe::boe2 {

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext);

} // namespace boe::boe2

#endif
