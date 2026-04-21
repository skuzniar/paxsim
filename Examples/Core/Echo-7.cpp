#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Acceptor.h"
#include "PaxSim/Core/Pipeline.h"
#include "PaxSim/Core/Streamlog.h"

#include "Echo-7/Source.h"
#include "Echo-7/Format.h"
#include "Echo-7/Target.h"

#include "Echo-7/Config.h"
#include "Echo-7/Context.h"

#include <iostream>

namespace PaxSim::Core {
logstream log(std::clog);
}

using namespace PaxSim;
using namespace PaxSim::Core; // For log

namespace {
void
usage(const char* program)
{
    std::cout << "Usage: " << program << " [-h: help] -c config" << '\n';
}
} // namespace

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    std::string cfgfile;

    int c = 0;
    while ((c = getopt(argc, argv, "c:h")) != -1) {
        switch (c) {
            case 'c':
                cfgfile = optarg;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    try {
        // Create configuration object by parsing the file
        Config config(cfgfile);

        // Get the log level
        const auto& cfglog = config["Log"];

        if (const std::string& level = cfglog["Level"]; !level.empty()) {
            switch (std::tolower(level[0])) {
                case 't':
                    Core::log << Core::threshold::trace;
                    break;
                case 'd':
                    Core::log << Core::threshold::debug;
                    break;
                case 'i':
                default:
                    Core::log << Core::threshold::info;
                    break;
                case 'w':
                    Core::log << Core::threshold::warning;
                    break;
                case 'e':
                    Core::log << Core::threshold::error;
                    break;
                case 'f':
                    Core::log << Core::threshold::fatal;
                    break;
            }
        }

        Core::IOContext context;

        using Context = Core::Aggregate<Text::Context>;

        // Configuration object will be passed to each context component
        Context ctx(config);

        using Handler = Core::Pipeline<Text::Source, Text::Format1, Text::Format2, Text::Target>;

        auto Acceptor = Core::Acceptor<Handler>(context, 7777);
        Acceptor.listen(config, ctx);
        context.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
