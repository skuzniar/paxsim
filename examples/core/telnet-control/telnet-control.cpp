#include "paxsim/core/acceptor.h"
#include "paxsim/core/iohandler.h"
#include "paxsim/core/hpipeline.h"
#include "paxsim/core/vpipeline.h"
#include "paxsim/core/streamlog.h"

#include "context.h"

#include "parser.h"
#include "format.h"
#include "filter.h"
#include "counter.h"
#include "timer.h"
#include "controller.h"
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
        boost::asio::io_context iocontx;
        boost::asio::signal_set signals(iocontx, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { iocontx.stop(); });

        // TODO - setup log stream to point to std::cerr, etc.
        core::log << threshold::info;

        // Message handler pipeline
        using MSGHandler = core::VPipeline 
                                < 
                                    core::HPipeline <core::VPipeline<text::Format, 
                                                                     text::Filter, 
                                                                     text::Counter>, text::Timer> 
                                >;
        using MSGIOHandler = core::IOHandler<text::Parser, MSGHandler, text::Writer, text::Controller>;

        // Control handler pipeline
        using CTLHandler = core::VPipeline 
                                < 
                                    text::Controller
                                >;
        using CTLIOHandler = core::IOHandler<text::Parser, CTLHandler, text::Writer>;

        text::Context context;

        auto msgAcceptor = core::Acceptor<MSGIOHandler, text::Context>(context, 7777, iocontx);
        auto ctlAcceptor = core::Acceptor<CTLIOHandler, text::Context>(context, 8888, iocontx);

        msgAcceptor.listen();
        ctlAcceptor.listen();

        iocontx.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
