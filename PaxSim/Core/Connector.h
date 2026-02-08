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
    Connector(const std::string& host, unsigned short port, IOContext& iocontx)
      : m_iocontx(iocontx)
      , m_endpoints(boost::asio::ip::tcp::resolver(iocontx.context()).resolve(host, std::to_string(port)))
    {
    }

    Connector(Context& context, const std::string& host, unsigned short port, IOContext& iocontx)
      : m_context(context)
      , m_iocontx(iocontx)
      , m_endpoints(boost::asio::ip::tcp::resolver(iocontx.context()).resolve(host, std::to_string(port)))
    {
    }

    std::weak_ptr<IOSession<IOHandler>>
    connect()
    {
        return async_connect_one();
    }

private:
    std::weak_ptr<IOSession<IOHandler>>
    async_connect_one()
    {
        if constexpr (std::is_same_v<Context, tag::Empty>) {
            return async_connect_one(std::make_shared<IOSession<IOHandler>>(m_iocontx));
        } else {
            return async_connect_one(std::make_shared<IOSession<IOHandler>>(m_context, m_iocontx));
        }
    }

    std::weak_ptr<IOSession<IOHandler>>
    async_connect_one(std::shared_ptr<IOSession<IOHandler>> session)
    {
        boost::asio::async_connect(session->socket(), m_endpoints, [this, session](boost::system::error_code e, boost::asio::ip::tcp::endpoint ep) {
            if (!e) {
                log << level::info << status << "Connected to " << ep << "..." << std::endl;
                session->start(session);
            } else {
                log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << e << ' '
                    << e.message() << std::endl;
            }
        });
        return session;
    }

    using context_t =
        std::conditional_t<std::is_same_v<Context, tag::Empty>, tag::Empty, std::reference_wrapper<Context>>;

    context_t                                    m_context;
    IOContext&                                   m_iocontx;
    boost::asio::ip::tcp::resolver::results_type m_endpoints;
};

} // namespace PaxSim::Core

#endif
