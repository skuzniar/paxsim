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
template<typename IOHandler>
class Connector
{
public:
    static constexpr std::size_t infinity = std::numeric_limits<std::size_t>::max();

    Connector(IOContext& iocontx, const std::string& host, unsigned short port, std::size_t retries = infinity, unsigned short delay = 1)
      : m_iocontxt(iocontx)
      , m_rctimer(iocontx.context())
      , m_endpoints(boost::asio::ip::tcp::resolver(iocontx.context()).resolve(host, std::to_string(port)))
      , m_retries(retries)
      , m_delay(delay)
    {
        m_iocontxt.attach(this);
    }

    ~Connector()
    {
        m_iocontxt.detach(this);
    }

    template<typename... Args>
    void connect(Args&&... args)
    {
        log << level::info << status << "Connecting..." << std::endl;
        async_connect_one(std::forward<Args>(args)...);
    }

private:
    template<typename... Args>
    void async_connect_one(Args&&... args)
    {
        log << level::trace << ts << here << std::endl;
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_iocontxt.context());

        boost::asio::async_connect(*socket, m_endpoints, [this, args = std::forward_as_tuple(args...), socket](auto error, auto endpoint) {
            std::apply(
                [this, &error, &socket, &endpoint](auto&&... params) {
                    if (!error) {
                        auto session = std::make_shared<IOSession<IOHandler>>(m_iocontxt, *socket, params...);
                        session->start(session);
                        m_iocontxt.detach(this);
                    } else {
                        log << level::error << ts << ' ' << error << ' ' << error.message() << std::endl;
                        if (m_retries-- > 0) {
                            m_rctimer.expires_after(boost::asio::chrono::seconds{ m_delay });
                            m_rctimer.async_wait([this, args = std::forward_as_tuple(params...)](auto error) {
                                std::apply([this, &error](auto&&... params) { async_connect_one(params...); }, args);
                            });
                        } else {
                            m_iocontxt.stop();
                        }
                    }
                },
                args);
        });
    }

    IOContext&                                   m_iocontxt;
    boost::asio::ip::tcp::resolver::results_type m_endpoints;
    boost::asio::steady_timer                    m_rctimer;
    std::size_t                                  m_retries = infinity;
    unsigned short                               m_delay   = 1;
};

} // namespace PaxSim::Core

#endif
