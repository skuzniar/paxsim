#include "Config.h"

#include "Fix/Fix42.h"

#include "PaxSim/Core/Streamlog.h"
#include <map>
#include <unistd.h>

namespace PaxSim::Core {
logstream log(std::clog);
}

namespace {
void
usage(const char* program)
{
    std::cerr << "Usage: " << program << " [-h: help] -c config" << '\n';
}

void
simulate(const std::string& type, Config& config)
{
    const static std::map<std::string, std::function<void(const Config&)>> simulators = { 
        // clang-format off
        { "Fix42", Fix::Fix42::simulate },
        // clang-format on
    };

    if (const auto itr = simulators.find(type); itr != simulators.end()) {
        itr->second(config);
    } else {
        std::cerr << "Unknown simulator type: " << type << '\n';
    }
}
} // namespace

using namespace PaxSim;

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

        const auto& simcfg = config["Simulator"];

        // Get the log level
        const auto& cfglog = simcfg["Log"];

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

        // Redirect log if needed
        if (const std::string& file = cfglog["File"]; !file.empty()) {
            Core::log << Core::level::info << "Redirecting log output to: " << file << '\n';
            Core::log.is(file);
        }

        // Run the simulator
        const std::string& simtype = simcfg["Type"];
        if (simtype.empty()) {
            std::cerr << "Missing simulator type ('Type') property in: " << cfgfile << '\n';
            return -1;
        }

        simulate(simtype, config);
    } catch (const std::exception& e) {
        std::cerr << "Simulator failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
