#include "Common/Config.h"

#include "FIX/FIX42Server.h"
#include "OUCH/OUCH50Server.h"

#include "PaxSim/Core/Streamlog.h"
#include <map>
#include <filesystem>
#include <unistd.h>

namespace PaxSim::Core {
logstream log(std::clog);
}

using namespace Common;

namespace {
void
usage(const char* program)
{
    std::cerr << "Usage: " << program << " [-h: help] -c config" << '\n';
}

using IOContext = PaxSim::Core::IOContext;

void
run(const std::string& type, const Config& config, IOContext& ioctx)
{
    const static std::map<std::string, std::function<void(IOContext&, const Config&)>> kinds = {
        // clang-format off
        { "FIX42",  FIX::FIX42::execute },
        { "OUCH50", OUCH::OUCH50::execute },
        // clang-format on
    };

    if (const auto itr = kinds.find(type); itr != kinds.end()) {
        itr->second(ioctx, config);
    } else {
        std::cerr << "Unknown Application type: " << type << '\n';
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

    std::filesystem::path cfgfile;

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

        const auto& appcfg = config["Application"];

        // Get the log level
        const auto& logcfg = appcfg["Log"];

        if (const std::string& level = logcfg["Level"]; !level.empty()) {
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
        if (const std::string& file = logcfg["File"]; !file.empty()) {
            Core::log << Core::level::info << "Redirecting log output to: " << file << '\n';
            Core::log.is(file);
        }

        // Run the application
        const std::string& simtype = appcfg["Type"];
        if (simtype.empty()) {
            std::cerr << "Missing Application type ('Type') property in: " << cfgfile << '\n';
            return -1;
        }

        IOContext ioctx;

        run(simtype, config, ioctx);
    } catch (const std::exception& e) {
        std::cerr << "Application failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
