#ifndef BOE_BOE3EQTServer_dot_h
#define BOE_BOE3EQTServer_dot_h

#include "PaxSim/Core/IOContext.h"
#include "Common/Config.h"

namespace boe::boe3::eqt {

void
run(const Common::Config& config, PaxSim::Core::IOContext& iocontext);

} // namespace boe::boe3::eqt

#endif
