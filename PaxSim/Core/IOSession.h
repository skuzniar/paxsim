#ifndef PaxSim_Core_IOSession_dot_h
#define PaxSim_Core_IOSession_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/RWBuffer.h>
#include <PaxSim/Core/IOContext.h>
#include <PaxSim/Core/Streamlog.h>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// IOSession performs reads and writes on input and output buffers, and runs the timer. IO and timeout notifications
// are forwarded to the IOHandler class.
//---------------------------------------------------------------------------------------------------------------------
template<typename IOHandler>
class IOSession
{
public:
    explicit IOSession(IOContext& iocontx)
      : m_handler(m_ibuf, m_obuf)
      , m_socket(iocontx.context())
      , m_timer(iocontx.context())

    {
        log << level::info << status << __func__ << '@' << this << std::endl;
    }

    template<typename Context>
    IOSession(Context& context, IOContext& iocontx)
      : m_handler(m_ibuf, m_obuf, context)
      , m_socket(iocontx.context())
      , m_timer(iocontx.context())
    {
        log << level::info << status << __func__ << '@' << this << std::endl;
    }

    IOSession(IOContext& iocontx, boost::asio::ip::tcp::socket socket)
      : m_handler(m_ibuf, m_obuf)
      , m_socket(std::move(socket))
      , m_timer(iocontx.context())
    {
        log << level::info << status << __func__ << '@' << this << std::endl;
    }

    template<typename Context>
    IOSession(Context& context, IOContext& iocontx, boost::asio::ip::tcp::socket socket)
      : m_handler(m_ibuf, m_obuf, context)
      , m_socket(std::move(socket))
      , m_timer(iocontx.context())
    {
        log << level::info << status << __func__ << '@' << this << std::endl;
    }

    ~IOSession()
    {
        log << level::info << status << __func__ << '@' << this << std::endl;
        try {
            m_timer.cancel();
        } catch (...) {
            ;
        }
    }

    void start(std::shared_ptr<IOSession> s)
    {
        start_io(s);
        start_timer(s);
    }

    /*
    boost::asio::ip::tcp::socket& socket()
    {
        return m_socket;
    }

    IOHandler& iohandler()
    {
        return m_handler;
    }
    */

private:
    bool ionotify()
    {
        const auto* begpos = m_ibuf.rpos();
        try {
            // Invoke input handler when there is data in the input buffer and enough space in the output buffer
            if (m_ibuf.rsize() > 0 && m_obuf.below_hw_mark()) {
                do {
                    begpos = m_ibuf.rpos();
                    m_handler.ioevent();
                } while (m_ibuf.rsize() > 0 && begpos != m_ibuf.rpos());
            }
        } catch (const std::exception& ex) {
            log << level::error << "IO failure: " << ex.what() << std::endl;
            return false;
        }
        return true;
    }

    bool notify_read()
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        return ionotify();
    }

    bool notify_write()
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        return ionotify();
    }

    void do_read(std::shared_ptr<IOSession> s)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        m_pending_read = true;
        m_socket.async_read_some(boost::asio::buffer(m_ibuf.wpos(), m_ibuf.wsize()),
                                 [s, this](boost::system::error_code e, std::size_t length) {
                                     m_pending_read = false;
                                     if (!e) {
                                         m_ibuf.produced(length);
                                         if (!notify_read()) {
                                             log << level::info << status << "Notify read failure." << std::endl;
                                             m_failed_read = true;
                                         }
                                     } else {
                                         log << level::error << "Read failure : " << e.message() << std::endl;
                                         m_failed_read = true;
                                     }
                                     start_io(s);
                                 });
    }

    void do_write(std::shared_ptr<IOSession> s)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        m_pending_write = true;
        m_socket.async_write_some(boost::asio::buffer(m_obuf.rpos(), m_obuf.rsize()),
                                  [s, this](boost::system::error_code e, std::size_t length) {
                                      m_pending_write = false;
                                      if (!e) {
                                          m_obuf.consumed(length);
                                          if (!notify_write()) {
                                              log << level::info << status << "Notify write failure." << std::endl;
                                              m_failed_write = true;
                                          }
                                      } else {
                                          log << level::error << "Write failure: " << e.message() << std::endl;
                                          m_failed_write = true;
                                      }
                                      start_io(s);
                                  });
    }

    void start_io(std::shared_ptr<IOSession> s)
    {
        if (!m_pending_read && !m_failed_read) {
            if (m_ibuf.above_hw_mark()) {
                m_ibuf.reset();
            }
            if (m_ibuf.wsize() > 0 && !m_ibuf.above_hw_mark()) {
                do_read(s);
            }
        }
        if (!m_pending_write && !m_failed_write) {
            if (m_obuf.above_hw_mark()) {
                m_obuf.reset();
            }
            if (m_obuf.rsize() > 0 && !m_obuf.above_hw_mark()) {
                do_write(s);
            }
        }

        if (!m_pending_read && !m_pending_write && (m_failed_read || m_failed_write)) {
            m_timer.cancel();
        }
    }

    void start_timer(std::shared_ptr<IOSession> s)
    {
        if (m_timepoint != timepoint::max()) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
            m_timer.expires_at(m_timepoint);
            m_timer.async_wait([this, s](const boost::system::error_code& e) { on_timeout(s, e); });
        }
    }

    void on_timeout(std::shared_ptr<IOSession> s, const boost::system::error_code& e)
    {
        if (e != boost::asio::error::operation_aborted) {
            try {
                m_timepoint = m_handler.timeout(std::chrono::steady_clock::now());
                log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
                start_io(s);
                start_timer(s);
            } catch (const std::exception& ex) {
                log << level::error << "Timeout failure." << std::endl;
                // TODO - flush output buffer
                m_socket.cancel();
            }
        }
    }

    IRWBuffer m_ibuf;
    ORWBuffer m_obuf;

    IOHandler m_handler;

    timepoint m_timepoint = m_handler.timeout(std::chrono::steady_clock::now());

    bool m_pending_read = false;
    bool m_failed_read  = false;

    bool m_pending_write = false;
    bool m_failed_write  = false;

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::steady_timer    m_timer;
};

} // namespace PaxSim::Core

#endif
