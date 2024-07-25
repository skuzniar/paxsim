#include "core/acceptor.h"
#include "core/context.h"
#include "core/iohandler.h"
#include "core/hpipeline.h"
#include "core/vpipeline.h"
#include "core/streamlog.h"

#include "Trading/Parser.h"
#include "Trading/Session.h"
#include "Trading/OrderBook.h"
#include "Trading/OrderBookAction.h"
#include "Trading/Pass.h"
#include "Trading/Writer.h"
#include "Trading/Controller.h"

#include "Control/Parser.h"
#include "Control/Controller.h"
#include "Control/Writer.h"

#include <boost/asio.hpp>
#include <json5cpp.h>
#include <iostream>
#include <fstream>

using namespace paxsim;
using namespace fix42;

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
                default:
                case 't':
                    core::log << core::threshold::trace;
                    break;
                case 'd':
                    core::log << core::threshold::debug;
                    break;
                case 'i':
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

        const auto& sescfg = cfg["Session"];
        const auto& ctlcfg = cfg["Control"];

        int sesport = sescfg["Port"].asInt();
        int ctlport = ctlcfg["Port"].asInt();

        if (sesport == 0 || ctlport == 0) {
            std::cerr << "Invalid configuration. Empty session and/or control port." << '\n';
            return 1;
        }

        boost::asio::io_context iocontx;
        boost::asio::signal_set signals(iocontx, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { iocontx.stop(); });

        // Session pipeline
        using SESHandler   = core::VPipeline<sim::Session, core::HPipeline<sim::OrderBook, sim::OrderBookAction, sim::Pass>>;
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
