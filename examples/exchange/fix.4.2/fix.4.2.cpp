#include "core/acceptor.h"
#include "core/context.h"
#include "core/iohandler.h"
#include "core/hpipeline.h"
#include "core/vpipeline.h"
#include "core/streamlog.h"

#include "sim/parser.h"
#include "sim/session.h"
#include "sim/orderbook.h"
#include "sim/pass.h"
#include "sim/writer.h"
#include "sim/controller.h"

#include "ctl/parser.h"
#include "ctl/controller.h"
#include "ctl/writer.h"

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using namespace paxsim;
using namespace paxsim::core; // For log
using namespace fix42;
//
using json = nlohmann::json;

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

    // Parse configuration file
    if (config.empty()) {
        std::cerr << "Empty configuration file." << '\n';
        return 1;
    }
    auto ifs = std::ifstream(config);
    if (!ifs) {
        std::cerr << "Unable to open configuration file: " << config << '\n';
        return 1;
    }
    json cfg = json::parse(ifs);
    if (cfg.empty()) {
        std::cerr << "Could not parse: " << config << '\n';
        return 1;
    }

    std::string level = cfg["Log"]["Level"];

    switch (std::tolower(level[0])) {
        default:
        case 'd':
            core::log << threshold::debug;
            break;
        case 'i':
            core::log << threshold::info;
            break;
        case 'w':
            core::log << threshold::warning;
            break;
        case 'e':
            core::log << threshold::error;
            break;
        case 'f':
            core::log << threshold::fatal;
            break;
    }

    /*
    std::unique_ptr<core::redirecter> rd;

    if (auto value = cfg["Log"]["File"]; value.is_string()) {
        std::string file = value;
        if (std::ofstream fstream(file); fstream) {
            std::cout << "Redirecting standard output to " << file << '\n';
            rd = std::make_unique<redirecter>(std::clog, fstream);
        }
    }
    */

    auto sescfg = cfg["Session"];
    auto ctlcfg = cfg["Control"];

    int sesport = sescfg["Port"];
    int ctlport = ctlcfg["Port"];

    if (sesport == 0 || ctlport == 0) {
        std::cerr << "Invalid configuration. Empty session and/or control port." << '\n';
        return 1;
    }

    try {
        boost::asio::io_context iocontx;
        boost::asio::signal_set signals(iocontx, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { iocontx.stop(); });

        // TODO - setup log stream to point to std::cerr, etc.
        core::log << threshold::info;

        // Session pipeline
        using SESHandler   = core::VPipeline<sim::Session, core::HPipeline<sim::OrderBook, sim::Pass>>;
        using SESIOHandler = core::IOHandler<sim::Parser, SESHandler, sim::Writer, sim::Controller>;

        // Control pipeline
        using CTLHandler   = core::VPipeline<ctl::Controller<sim::ControllerContext>>;
        using CTLIOHandler = core::IOHandler<ctl::Parser, CTLHandler, ctl::Writer>;

        using Context = core::Context<sim::SessionContext, sim::OrderBookContext, sim::ControllerContext>;

        Context context(cfg);

        auto sesAcceptor = core::Acceptor<SESIOHandler, Context>(context, sesport, iocontx);
        auto ctlAcceptor = core::Acceptor<CTLIOHandler, Context>(context, ctlport, iocontx);

        sesAcceptor.listen();
        ctlAcceptor.listen();

        iocontx.run();

        std::cout << "Server done." << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Server failure: " << e.what() << '.' << '\n';
    }

    return 0;
}
