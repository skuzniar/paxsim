#include "core/acceptor.h"
#include "core/context.h"
#include "core/iohandler.h"
#include "core/hpipeline.h"
#include "core/vpipeline.h"

#include "Fix42/Trading/Parser.h"
#include "Fix42/Trading/Session.h"
#include "Fix42/Trading/OrderBook.h"
#include "Fix42/Trading/OrderBookAction.h"
#include "Fix42/Trading/Pass.h"
#include "Fix42/Trading/Writer.h"
#include "Fix42/Trading/Controller.h"

#include "Fix42/Control/Parser.h"
#include "Fix42/Control/Controller.h"
#include "Fix42/Control/Writer.h"

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

    // Session pipeline
    using SESHandler =
        core::VPipeline<Trading::Session, core::HPipeline<Trading::OrderBook, Trading::OrderBookAction, Trading::Pass>>;
    using SESIOHandler = core::IOHandler<Trading::Parser, SESHandler, Trading::Writer, Trading::Controller>;

    // Control pipeline
    using CTLHandler   = core::VPipeline<Control::Controller<Trading::ControllerContext>>;
    using CTLIOHandler = core::IOHandler<Control::Parser, CTLHandler, Control::Writer>;

    using Context = core::Context<Trading::SessionContext, Trading::OrderBookContext, Trading::ControllerContext>;

    Context context(cfg);

    auto sesAcceptor = core::Acceptor<SESIOHandler, Context>(context, sesport, iocontext);
    auto ctlAcceptor = core::Acceptor<CTLIOHandler, Context>(context, ctlport, iocontext);

    sesAcceptor.listen();
    ctlAcceptor.listen();

    iocontext.run();
}

} // namespace Fix42
