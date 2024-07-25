#ifndef Exchange_Simulator_Fix42_dot_h
#define Exchange_Simulator_Fix42_dot_h

#include <json5cpp.h>
#include <boost/asio.hpp>

namespace Fix42 {

using json = Json::Value;

void
simulate(const json& cfg, boost::asio::io_context& iocontext);

} // namespace Fix42

#endif
