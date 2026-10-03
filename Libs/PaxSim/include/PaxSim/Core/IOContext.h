#ifndef PaxSim_Core_IOContext_dot_h
#define PaxSim_Core_IOContext_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/Streamlog.h>

#include <boost/asio.hpp>
#include <set>

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

    void attach(const void* it)
    {
        if (m_set.insert(reinterpret_cast<std::size_t>(it)).second) {
            log << level::debug << status << "Attached " << it << ' ' << m_set.size() << std::endl;
        }
    }
    void detach(const void* it)
    {
        if (m_set.erase(reinterpret_cast<std::size_t>(it)) > 0) {
            log << level::debug << status << "Detached " << it << ' ' << m_set.size() << std::endl;
            if (m_set.empty()) {
                m_context.stop();
            }
        }
    }

    void run()
    {
        log << level::debug << status << "Starting..." << std::endl;
        m_context.run();
    }

    void stop()
    {
        log << level::debug << status << "Stopping..." << std::endl;
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
    std::set<std::size_t>   m_set;
};

} // namespace PaxSim::Core

#endif
