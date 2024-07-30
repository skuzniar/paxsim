#include "core/acceptor.h"
#include "core/iohandler.h"
#include "core/hpipeline.h"
#include "core/streamlog.h"

#include "parser.h"
#include "timer.h"
#include "filter.h"
#include "writer.h"

#include <boost/asio.hpp>
#include <iostream>

namespace paxsim::core {
logstream log(std::clog);
}

using namespace paxsim;
using namespace paxsim::core; // For log

int
main(int argc, char* argv[])
{
    try {
        boost::asio::io_context context;
        boost::asio::signal_set signals(context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { context.stop(); });

        // TODO - setup log stream to point to std::cerr, etc.
        core::log << threshold::info;

        using Handler   = core::HPipeline<text::Filter, text::Timer>;
        using IOHandler = core::IOHandler<text::Parser, Handler, text::Writer>;

        auto acceptor = core::Acceptor<IOHandler>(7777, context);
        acceptor.listen();
        context.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
