#ifndef Paxsim_Core_Connector_dot_h
#define Paxsim_Core_Connector_dot_h

#include "iosession.h"
#include "streamlog.h"

#include <type_traits>
#include <boost/asio.hpp>

namespace paxsim::core {

//---------------------------------------------------------------------------------------------------------------------
// Connector class is responsible for connecting a socket and handing it off to IOSession. IOSession uses Handler class
// to process IO and timer events. Handlers are produced by the Factory class and Connector uses that class to make a
// new handler once the connection has been established.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Connector
{
public:
    Connector(Factory& factory, const std::string& host, unsigned short port, boost::asio::io_context& context)
      : m_factory(factory)
      , m_address(boost::asio::ip::address::from_string(host), port)
      , m_context(context)
      , m_socket(m_context)
    {
    }

    void connect()
    {
        log::info("Connecting to ", m_address.address(), ':', m_address.port(), "...", '\n');
        async_connect_one();
    }

private:
    void async_connect_one()
    {
        m_socket.async_connect(m_address, [this](boost::system::error_code e) {
            if (!e) {
                auto* handler = m_factory.handler();
                using Handler = std::remove_pointer_t<decltype(handler)>;
                using Session = typename paxsim::core::IOSession<Handler>;
                auto session  = std::make_shared<Session>(m_context, std::move(m_socket), handler);

                session->start(session);
            } else {
                log::error(__FILE__, ':', __LINE__, ": ", e, ' ', e.message(), '\n');
                m_context.stop();
            }
        });
    }

    Factory&                       m_factory;
    boost::asio::ip::tcp::endpoint m_address;
    boost::asio::io_context&       m_context;
    boost::asio::ip::tcp::socket   m_socket;
};

} // namespace paxsim::core

#endif
