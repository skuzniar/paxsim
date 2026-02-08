#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Streamlog.h"

#include "Handler.h"

#include <boost/asio.hpp>
#include <iostream>

namespace PaxSim::Core {
logstream log(std::clog);
}

using namespace PaxSim;
using namespace PaxSim::Core; // For log

int
main(int argc, char* argv[])
{
    if (argc > 1) {
        switch (argv[1][0]) {
            case 't':
                Core::log << threshold::trace;
                break;
            case 'd':
                Core::log << threshold::debug;
                break;
            case 'i':
            default:
                Core::log << threshold::info;
                break;
        }
    } else {
        Core::log << threshold::info;
    }

    try {
        Core::IOContext context;

        // TODO - setup log stream to point to std::cerr, etc.
        Core::log << threshold::info;

        using Handler = Core::Pipeline<Text::Handler>;

        auto Acceptor = Core::Acceptor<Handler>(7777, context);
        Acceptor.listen();
        context.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
