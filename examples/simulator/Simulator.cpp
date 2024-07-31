#include <paxsim/core/streamlog.h>

#include "Fix42/Fix42.h"

#include <boost/asio.hpp>
#include <json5cpp.h>
#include <iostream>
#include <fstream>

namespace paxsim::core {
logstream log(std::clog);
}

using namespace paxsim;

using namespace Fix42;

using json = Json::Value;

void
usage(const char* program)
{
    std::cerr << "Usage: " << program << " [-h: help] -c config" << '\n';
}

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    std::string config;

    int c = 0;
    while ((c = getopt(argc, argv, "c:h")) != -1) {
        switch (c) {
            case 'c':
                config = optarg;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if (config.empty()) {
        std::cerr << "Empty configuration file." << '\n';
        return 1;
    }

    // Parse configuration file
    auto ifs = std::ifstream(config);
    if (!ifs) {
        std::cerr << "Unable to open configuration file: " << config << '\n';
        return 1;
    }

    try {
        json cfg;

        std::string err;
        if (!Json5::parse(ifs, cfg, &err)) {
            std::cerr << "Could not parse: " << config << ' ' << "error: " << err << '\n';
            return 1;
        }

        const auto& cfglog = cfg["Log"];

        if (const auto& level = cfglog["Level"]) {
            switch (std::tolower(level.asString()[0])) {
                case 't':
                    core::log << core::threshold::trace;
                    break;
                case 'd':
                    core::log << core::threshold::debug;
                    break;
                case 'i':
                default:
                    core::log << core::threshold::info;
                    break;
                case 'w':
                    core::log << core::threshold::warning;
                    break;
                case 'e':
                    core::log << core::threshold::error;
                    break;
                case 'f':
                    core::log << core::threshold::fatal;
                    break;
            }
        }

        if (const auto& file = cfglog["File"]) {
            core::log << core::level::info << "Redirecting log output to: " << file << '\n';
            core::log.is(file.asString());
        }

        boost::asio::io_context iocontext;
        boost::asio::signal_set signals(iocontext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { iocontext.stop(); });

        // This is a blocking call
        Fix42::simulate(cfg, iocontext);

        std::cout << "Simulator done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Simulator failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
