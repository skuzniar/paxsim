#ifndef Paxsim_Core_Acceptor_dot_h
#define Paxsim_Core_Acceptor_dot_h

#include <paxsim/core/iosession.h>
#include <paxsim/core/streamlog.h>
#include <paxsim/core/types.h>

#include <boost/asio.hpp>

namespace paxsim::core {

//---------------------------------------------------------------------------------------------------------------------
// Acceptor is responsible for accepting socket connections and creating IOSession objects.
//---------------------------------------------------------------------------------------------------------------------
template<typename IOHandler, typename Context = tag::Empty>
class Acceptor
{
public:
    Acceptor(unsigned short port, boost::asio::io_context& iocontx)
      : m_iocontx(iocontx)
      , m_acceptor{ m_iocontx, { boost::asio::ip::tcp::v4(), port } }
    {
    }

    Acceptor(Context& context, unsigned short port, boost::asio::io_context& iocontx)
      : m_context(context)
      , m_iocontx(iocontx)
      , m_acceptor{ m_iocontx, { boost::asio::ip::tcp::v4(), port } }
    {
    }

    void listen()
    {
        log << level::info << status << "Listening on port " << m_acceptor.local_endpoint().port() << "..."
            << std::endl;
        async_accept_one();
    }

private:
    void async_accept_one()
    {
        m_acceptor.async_accept([this](boost::system::error_code e, boost::asio::ip::tcp::socket socket) {
            if (!e) {
                using Session = IOSession<IOHandler>;
                if constexpr (std::is_same_v<Context, tag::Empty>) {
                    auto session = std::make_shared<Session>(m_iocontx, std::move(socket));
                    session->start(session);
                } else {
                    auto session = std::make_shared<Session>(m_context, m_iocontx, std::move(socket));
                    session->start(session);
                }
            } else {
                log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << e << ' ' << e.message()
                    << std::endl;
            }

            async_accept_one();
        });
    }

    using context_t =
        std::conditional_t<std::is_same_v<Context, tag::Empty>, tag::Empty, std::reference_wrapper<Context>>;

    context_t                      m_context;
    boost::asio::io_context&       m_iocontx;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace paxsim::core

#endif
