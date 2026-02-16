#ifndef PaxSim_Core_Connector_dot_h
#define PaxSim_Core_Connector_dot_h

#include <PaxSim/Core/IOContext.h>
#include <PaxSim/Core/IOSession.h>
#include <PaxSim/Core/Streamlog.h>
#include <PaxSim/Core/Types.h>

#include <boost/asio.hpp>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// Connector is responsible for connecting a socket and creating IOSession objects.
//---------------------------------------------------------------------------------------------------------------------
template<typename IOHandler, typename Context = tag::Empty>
class Connector
{
public:
    static constexpr std::size_t infinity = std::numeric_limits<std::size_t>::max();

    Connector(IOContext& iocontx, const std::string& host, unsigned short port, std::size_t retries = infinity, unsigned short delay = 1)
      : m_iocontx(iocontx)
      , m_rctimer(iocontx.context())
      , m_endpoints(boost::asio::ip::tcp::resolver(iocontx.context()).resolve(host, std::to_string(port)))
      , m_retries(retries)
      , m_delay(delay)
    {
    }

    Connector(IOContext& iocontx, Context& context, const std::string& host, unsigned short port, std::size_t retries = infinity, unsigned short delay = 1)
      : m_iocontx(iocontx)
      , m_context(context)
      , m_rctimer(iocontx.context())
      , m_endpoints(boost::asio::ip::tcp::resolver(iocontx.context()).resolve(host, std::to_string(port)))
      , m_retries(retries)
      , m_delay(delay)
    {
    }

    void connect()
    {
        log << level::info << status << "Connecting..." << std::endl;
        return async_connect_one();
    }

private:
    void async_connect_one()
    {
        log << level::trace << ts << here << ' ' << __func__ << std::endl;
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_iocontx.context());

        boost::asio::async_connect(*socket, m_endpoints, [this, s = socket](boost::system::error_code e, boost::asio::ip::tcp::endpoint ep) {
            if (!e) {
                log << level::info << status << "Connected to " << ep << std::endl;
                if constexpr (std::is_same_v<Context, tag::Empty>) {
                    auto session = std::make_shared<IOSession<IOHandler>>(m_iocontx, std::move(*s));
                    session->start(session);
                } else {
                    auto session = std::make_shared<IOSession<IOHandler>>(m_context, m_iocontx, std::move(*s));
                    session->start(session);
                }
            } else {
                log << level::info << status << "Failed to connect." << ' ' << e.message() << std::endl;
                s->close();
                if (m_retries-- > 0) {
                    m_rctimer.expires_at(std::chrono::steady_clock::now() + std::chrono::seconds(m_delay));
                    m_rctimer.async_wait([this](const boost::system::error_code& e) { async_connect_one(); });
                }
            }
        });
    }

    using context_t = std::conditional_t<std::is_same_v<Context, tag::Empty>, tag::Empty, std::reference_wrapper<Context>>;

    IOContext&                                   m_iocontx;
    context_t                                    m_context;
    boost::asio::ip::tcp::resolver::results_type m_endpoints;
    boost::asio::steady_timer                    m_rctimer;
    std::size_t                                  m_retries = infinity;
    unsigned short                               m_delay   = 1;
};

} // namespace PaxSim::Core

#endif
