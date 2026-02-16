#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Context.h"

#include "Source.h"
#include "Format.h"
#include "Target.h"

#include "Config.h"
#include "Context.h"

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

        // Create configuration object by parsing command line options
        Text::Config cfg(argc, argv);
        
        using Context = Core::Context<Text::Context>;

        // Configuration object will be passed to each context component
        Context ctx(cfg);

        using Handler = Core::Pipeline<Text::Source, Text::Format1, Text::Format2, Text::Target>;

        auto Acceptor = Core::Acceptor<Handler, Context>(context, ctx, 7777);
        Acceptor.listen();
        context.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
