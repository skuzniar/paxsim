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
template<typename IOHandler>
class Acceptor
{
public:
    Acceptor(IOContext& iocontext, unsigned short port)
      : m_iocontxt(iocontext)
      , m_acceptor{ m_iocontxt.context(), { boost::asio::ip::tcp::v4(), port } }
    {
        m_iocontxt.attach(this);
    }

    ~Acceptor()
    {
        m_iocontxt.detach(this);
    }

    template<typename... Args>
    void listen(Args&&... args)
    {
        log << level::info << status << "Listening on port " << m_acceptor.local_endpoint().port() << "..." << std::endl;
        async_accept_one(std::forward<Args>(args)...);
    }

private:
    template<typename... Args>
    void async_accept_one(Args&&... args)
    {
        m_acceptor.async_accept([this, args = std::forward_as_tuple(args...)](auto error, auto socket) {
            std::apply(
                [this, &error, &socket](auto&&... params) {
                    if (!error) {
                        auto session = std::make_shared<IOSession<IOHandler>>(m_iocontxt, socket, params...);
                        session->start(session);
                    } else {
                        log << level::trace << ts << here << ' ' << error << ' ' << error.message() << std::endl;
                    }
                    async_accept_one(params...);
                },
                args);
        });
    }

    IOContext&                     m_iocontxt;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace PaxSim::Core

#endif
