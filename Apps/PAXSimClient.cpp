#include "Common/Config.h"

#include "FIX/FIX42Client.h"
#include "OUCH/OUCH50Client.h"
#include "BOE/BOE2Client.h"

#include "PaxSim/Core/IOContext.h"
#include "PaxSim/Core/Streamlog.h"

#include <filesystem>
#include <unistd.h>

namespace PaxSim::Core {
Logstream log(std::clog);
}

namespace {
void
usage(const char* program)
{
    std::cerr << "Usage: " << program << " [-h: help] -c config" << '\n';
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
        Common::Config config(cfgfile);

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

        // Select the type of client to run
        const std::string& type = appcfg["Type"];
        if (type.empty()) {
            std::cerr << "Missing Client type ('Type') property in: " << cfgfile << '\n';
            return -1;
        }

        Core::IOContext iocontext;

        std::thread thread;
        if (type == "FIX42") {
            thread = std::thread([&]() { FIX::FIX42::run(config, iocontext); });
        }
        if (type == "OUCH50") {
            thread = std::thread([&]() { OUCH::OUCH50::run(config, iocontext); });
        }
        if (type == "BOE2") {
            thread = std::thread([&]() { BOE::BOE2::run(config, iocontext); });
        }

        char line[100];
        do {
            std::cout << "===> ";
            std::cin.getline(line, 100);
        } while (std::cin.clear(), line[0] != 'q');

        iocontext.stop();
        thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Application failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
