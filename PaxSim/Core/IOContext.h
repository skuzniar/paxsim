#ifndef PaxSim_Core_IOContext_dot_h
#define PaxSim_Core_IOContext_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/Streamlog.h>

#include <boost/asio.hpp>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// IOContext is a thin wrapper around asio::io_context class.
//---------------------------------------------------------------------------------------------------------------------
class IOContext
{
public:
    explicit IOContext()
      : m_signals(m_context, SIGINT, SIGTERM)
    {
        m_signals.async_wait([&](auto, auto) { m_context.stop(); });
    }

    boost::asio::io_context& context()
    {
        return m_context;
    }

    void run()
    {
        m_context.run();
    }

    void stop()
    {
        m_context.stop();
    }

    template<typename F>
    void post(F&& f)
    {
        boost::asio::post(m_context, f);
    }

private:
    boost::asio::io_context m_context;
    boost::asio::signal_set m_signals;
};

} // namespace PaxSim::Core

#endif
