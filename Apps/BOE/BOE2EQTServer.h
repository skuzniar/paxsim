#ifndef BOE_BOE2Server_dot_h
#define BOE_BOE2Server_dot_h

#include "PaxSim/Core/IOContext.h"
#include "Common/Config.h"

namespace BOE::BOE2 {

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext);

} // namespace BOE::BOE2

#endif
