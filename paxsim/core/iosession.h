#ifndef Paxsim_Core_IOSession_dot_h
#define Paxsim_Core_IOSession_dot_h

#include <paxsim/core/types.h>
#include <paxsim/core/rwbuffer.h>
#include <paxsim/core/streamlog.h>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace paxsim::core {

//---------------------------------------------------------------------------------------------------------------------
// IOSession performs reads and writes on input and output buffers, and runs the timer. IO and timeout notifications
// are forwarded the the IOHandler class.
//---------------------------------------------------------------------------------------------------------------------
template<typename IOHandler>
class IOSession
{
public:
    explicit IOSession(boost::asio::io_context& iocontx, boost::asio::ip::tcp::socket socket)
      : m_socket(std::move(socket))
      , m_timer(iocontx)
    {
        log << level::info << status << __func__ << '@' << this << std::endl;
    }

    template<typename Context>
    explicit IOSession(Context& context, boost::asio::io_context& iocontx, boost::asio::ip::tcp::socket socket)
      : m_handler(context)
      , m_socket(std::move(socket))
      , m_timer(iocontx)
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

private:
    bool notify_read(RWBuffer& ibuff, RWBuffer& obuff)
    {
        // log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        bool  action = true;
        auto* begpos = ibuff.rpos();
        do {
            begpos = ibuff.rpos();
            action = m_handler.notify(ibuff, obuff);
        } while (action && begpos != ibuff.rpos());
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << action << std::endl;
        return action;
    }

    bool notify_write(RWBuffer& ibuff, RWBuffer& obuff)
    {
        // log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        bool  action = true;
        auto* begpos = obuff.rpos();
        do {
            action = m_handler.notify(ibuff, obuff);
        } while (action && begpos != obuff.rpos());
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << action << std::endl;
        return action;
    }

    void do_read(std::shared_ptr<IOSession> s)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        m_pending_read = true;
        m_socket.async_read_some(boost::asio::buffer(m_ibuff.wpos(), m_ibuff.wsize()),
                                 [s, this](boost::system::error_code e, std::size_t length) {
                                     m_pending_read = false;
                                     if (!e) {
                                         m_ibuff.produced(length);
                                         if (!notify_read(m_ibuff, m_obuff)) {
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
        m_socket.async_write_some(boost::asio::buffer(m_obuff.rpos(), m_obuff.rsize()),
                                  [s, this](boost::system::error_code e, std::size_t length) {
                                      m_pending_write = false;
                                      if (!e) {
                                          m_obuff.consumed(length);
                                          if (!notify_write(m_ibuff, m_obuff)) {
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
            if (m_ibuff.above_hw_mark()) {
                m_ibuff.reset();
            }
            if (m_ibuff.wsize() > 0 && !m_ibuff.above_hw_mark()) {
                do_read(s);
            }
        }
        if (!m_pending_write && !m_failed_write) {
            if (m_obuff.above_hw_mark()) {
                m_obuff.reset();
            }
            if (m_obuff.rsize() > 0 && !m_obuff.above_hw_mark()) {
                do_write(s);
            }
        }

        if (!m_pending_read && !m_pending_write && (m_failed_read || m_failed_write)) {
            m_timer.cancel();
        }
    }

    void start_timer(std::shared_ptr<IOSession> s)
    {
        auto nexttp = std::min(m_timeout_timepoint, m_control_timepoint);
        if (nexttp != timepoint::max()) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
            m_timer.expires_at(nexttp);
            if (nexttp == m_timeout_timepoint) {
                m_timer.async_wait([this, s](const boost::system::error_code& e) { on_timeout(s, e); });
            } else {
                m_timer.async_wait([this, s](const boost::system::error_code& e) { on_control(s, e); });
            }
        }
    }

    void on_timeout(std::shared_ptr<IOSession> s, const boost::system::error_code& e)
    {
        if (e != boost::asio::error::operation_aborted) {
            if (m_handler.timeout(m_ibuff, m_obuff)) {
                m_timeout_timepoint = m_handler.timeout_at();
                start_io(s);
                start_timer(s);
            } else {
                log << level::error << "Timeout failure." << std::endl;
                // TODO - flush output buffer
                m_socket.cancel();
            }
        }
    }

    void on_control(std::shared_ptr<IOSession> s, const boost::system::error_code& e)
    {
        if (e != boost::asio::error::operation_aborted) {
            if (m_handler.control(m_ibuff, m_obuff)) {
                m_control_timepoint = m_handler.control_at();
                start_io(s);
                start_timer(s);
            } else {
                log << level::error << "Control failure." << std::endl;
                // TODO - flush output buffer
                m_socket.cancel();
            }
        }
    }

    IOHandler m_handler;

    timepoint m_timeout_timepoint = m_handler.timeout_at();
    timepoint m_control_timepoint = m_handler.control_at();

    RWBuffer m_ibuff;
    RWBuffer m_obuff;

    bool m_pending_read = false;
    bool m_failed_read  = false;

    bool m_pending_write = false;
    bool m_failed_write  = false;

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::steady_timer    m_timer;
};

} // namespace paxsim::core

#endif
