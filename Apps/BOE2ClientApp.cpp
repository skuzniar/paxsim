#include "BOE2Client.h"

#include <unistd.h>

namespace PaxSim::Core {
Logstream log(std::clog);
}

void
usage(const char* program)
{
    std::cerr << "Usage: " << program
              << "-h host -p port -u user -w password "
                 "[-l log-level (d - debug, i - info, e - error)] [-f log-file]"
              << std::endl;
}

int
main(int argc, char* argv[])
{
    std::string host     = "localhost";
    int         port     = 7777;
    std::string user     = "PAXUSR";
    std::string password = "PAXPAS";
    std::string loglevel = "debug";
    std::string logfile;

    int c = 0;
    while ((c = getopt(argc, argv, "h:p:u:w:l:f")) != -1) {
        switch (c) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                port = std::atoi(optarg);
                break;
            case 'u':
                user = optarg;
                break;
            case 'w':
                password = optarg;
                break;
            case 'l':
                loglevel = optarg;
                break;
            case 'f':
                logfile = optarg;
                break;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if (!loglevel.empty()) {
        switch (std::tolower(loglevel[0])) {
            case 'd':
                Core::log << Core::threshold::debug;
                break;
            case 'i':
            default:
                Core::log << Core::threshold::info;
                break;
            case 'e':
                Core::log << Core::threshold::error;
                break;
        }
    }

    if (!logfile.empty()) {
        Core::log << Core::level::info << "Redirecting log output to: " << logfile << '\n';
        Core::log.is(logfile);
    }

    try {
        Client client(host, port, user, password);

        client.start();

        auto prompt = []() {
            std::cout << "h - Print this message." << '\n'
                      << "n - Send new order." << '\n'
                      << "r - Send replace order." << '\n'
                      << "c - Send cancel order." << '\n'
                      << "q - Quit." << std::endl;
        };

        char line[32];
        do {
            std::cout << "===> ";
            std::cin.getline(line, 32);

            switch (line[0]) {
                default:
                case 'h':
                    prompt();
                    break;
                case 'n':
                    client.post();
                    break;
                case 'r':
                    break;
                case 'c':
                    break;
                case 'q':
                    break;
            };
        } while (std::cin.clear(), line[0] != 'q');

        client.stop();
    } catch (const std::exception& e) {
        std::cerr << "Application failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
