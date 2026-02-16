#ifndef PaxSim_Core_Acceptor_dot_h
#define PaxSim_Core_Acceptor_dot_h

#include <PaxSim/Core/IOContext.h>
#include <PaxSim/Core/IOSession.h>
#include <PaxSim/Core/Streamlog.h>
#include <PaxSim/Core/Types.h>

#include <boost/asio.hpp>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// Acceptor is responsible for accepting socket connections and creating IOSession objects.
//---------------------------------------------------------------------------------------------------------------------
template<typename IOHandler, typename Context = tag::Empty>
class Acceptor
{
public:
    Acceptor(IOContext& iocontx, unsigned short port)
      : m_iocontx(iocontx)
      , m_acceptor{ m_iocontx.context(), { boost::asio::ip::tcp::v4(), port } }
    {
    }

    Acceptor(IOContext& iocontx, Context& context, unsigned short port)
      : m_iocontx(iocontx)
      , m_context(context)
      , m_acceptor{ m_iocontx.context(), { boost::asio::ip::tcp::v4(), port } }
    {
    }

    void listen()
    {
        log << level::info << status << "Listening on port " << m_acceptor.local_endpoint().port() << "..." << std::endl;
        async_accept_one();
    }

private:
    void async_accept_one()
    {
        m_acceptor.async_accept([this](boost::system::error_code e, boost::asio::ip::tcp::socket socket) {
            if (!e) {
                if constexpr (std::is_same_v<Context, tag::Empty>) {
                    auto session = std::make_shared<IOSession<IOHandler>>(m_iocontx, std::move(socket));
                    session->start(session);
                } else {
                    auto session = std::make_shared<IOSession<IOHandler>>(m_context, m_iocontx, std::move(socket));
                    session->start(session);
                }
            } else {
                log << level::trace << ts << here << ' ' << __func__ << ' ' << e << ' ' << e.message() << std::endl;
            }

            async_accept_one();
        });
    }

    using context_t = std::conditional_t<std::is_same_v<Context, tag::Empty>, tag::Empty, std::reference_wrapper<Context>>;

    IOContext&                     m_iocontx;
    context_t                      m_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace PaxSim::Core

#endif
