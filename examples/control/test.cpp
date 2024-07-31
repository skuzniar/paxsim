#include "paxsim/control/command.h"

#include <json5cpp.h>
#include <fstream>

using namespace paxsim;

using json = Json::Value;

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command-file>" << std::endl;
        return 1;
    }

    auto ifs = std::ifstream(argv[1]);
    if (!ifs) {
        std::cerr << "Unable to open configuration file: " << argv[1] << '\n';
        return 1;
    }

    try {
        json        cmd;
        std::string err;
        if (Json5::parse(ifs, cmd, &err)) {
            std::cout << '[' << control::Factory::command(cmd) << ']' << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Test failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
