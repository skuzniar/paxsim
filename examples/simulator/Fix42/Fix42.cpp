#include <paxsim/core/acceptor.h>
#include <paxsim/core/context.h>
#include <paxsim/core/iohandler.h>
#include <paxsim/core/hpipeline.h>
#include <paxsim/core/vpipeline.h>

#include "Trading/Parser.h"
#include "Trading/Session.h"
#include "Trading/OrderBookFilter.h"
#include "Trading/OrderBookHandler.h"
#include "Trading/OrderBookControl.h"
#include "Trading/ExecutionBookHandler.h"
#include "Trading/ExecutionBookControl.h"
#include "Trading/Pass.h"
#include "Trading/Writer.h"
#include "Trading/Controller.h"

#include "Command/Parser.h"
#include "Command/Controller.h"
#include "Command/Writer.h"

#include "Fix42.h"

#include <boost/asio.hpp>
#include <json5cpp.h>
#include <iostream>

namespace Fix42 {

void
simulate(const json& cfg, boost::asio::io_context& iocontext)
{
    using namespace paxsim;
    using namespace Fix42;

    using json = Json::Value;

    const auto& sescfg = cfg["Session"];
    const auto& ctlcfg = cfg["Control"];

    int sesport = sescfg["Port"].asInt();
    int ctlport = ctlcfg["Port"].asInt();

    if (sesport == 0 || ctlport == 0) {
        std::cerr << "Invalid configuration. Empty session and/or control port." << '\n';
        return;
    }

    // Order Book Filter followed by Order Book Handler
    using OBKHandler = core::VPipeline<Trading::OrderBookFilter, Trading::OrderBookHandler>;

    // Trading pipeline
    using TRDHandler   = core::VPipeline<Trading::Session,
                                         core::HPipeline<OBKHandler,
                                                         Trading::OrderBookControl,
                                                         Trading::ExecutionBookHandler,
                                                         Trading::ExecutionBookControl,
                                                         Trading::Pass>>;
    using TRDIOHandler = core::IOHandler<Trading::Parser, TRDHandler, Trading::Writer, Trading::Controller>;

    // Command pipeline
    using CMDIOHandler = core::IOHandler<Command::Parser, Command::Controller, Command::Writer>;

    using Context = core::Context<Trading::SessionContext, Trading::OrderBookContext, Trading::ControllerContext>;

    Context context(cfg);

    auto sesAcceptor = core::Acceptor<TRDIOHandler, Context>(context, sesport, iocontext);
    auto ctlAcceptor = core::Acceptor<CMDIOHandler, Context>(context, ctlport, iocontext);

    sesAcceptor.listen();
    ctlAcceptor.listen();

    iocontext.run();
}

} // namespace Fix42
