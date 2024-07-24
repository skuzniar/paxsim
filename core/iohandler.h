#ifndef Paxsim_Core_IOHandler_dot_h
#define Paxsim_Core_IOHandler_dot_h

#include "core/streamlog.h"
#include "core/rwbuffer.h"
#include "core/types.h"

#include <cassert>
#include <type_traits>

namespace paxsim::core {

//----------------------------------------------------------------------------------------------------------------------
// IOHandler handles messages and timer events. It uses Parser to turn raw bytes from the input buffer into a message.
// Controller is a source of control commands. Message and command processing is performed by the Handler. Handler
// consumes incoming messages and commands and produces zero or more outgoing messages. Writer puts outgoing messages
// into the output buffer. Timer events work in a similar way. The Handler receives timer event and produces zero or
// more messages. Then the process continues as with the message processing.
//----------------------------------------------------------------------------------------------------------------------
template<typename Parser, typename Handler, typename Writer, typename Controller = tag::Empty>
class IOHandler
{
public:
    IOHandler()
    {
    }

    template<typename Context>
    explicit IOHandler(Context& context)
      : m_parser(context)
      , m_handler(context)
      , m_writer(context)
      , m_controller(context)
    {
    }

    // Return the next expiration point at which the handler wants the timeout timer to go off.
    [[nodiscard]] timepoint timeout_at() const
    {
        if constexpr (tag::is_cyclical<std::remove_reference_t<decltype(m_handler)>>()) {
            return m_handler.timeout_at();
        } else {
            return timepoint::max();
        }
    }

    // Return the next expiration point at which the handler wants the control timer to go off.
    [[nodiscard]] timepoint control_at() const
    {
        if constexpr (!std::is_same_v<Controller, tag::Empty>) {
            return m_controller.control_at();
        } else {
            return timepoint::max();
        }
    }

    // Invoke IO callback.
    bool notify(RWBuffer& ibuf, RWBuffer& obuf)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;

        auto [result, size, message] = m_parser.parse(ibuf.rpos(), ibuf.rsize());
        while (result && size > 0) {
            log << level::trace << ts << ' ' << '[' << hexdump(ibuf.rpos(), size) << ']' << std::endl;
            ibuf.consumed(size);

            for (auto msg : m_handler.process(std::move(message))) {
                if (msg) {
                    if (!write(*msg, obuf)) {
                        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << false
                            << std::endl;
                        return false;
                    }
                } else {
                    log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << false
                        << std::endl;
                    return false;
                }
            }
            std::tie(result, size, message) = m_parser.parse(ibuf.rpos(), ibuf.rsize());
        }

        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << true << ' '
            << std::endl;
        return true;
    }

    // Invoke control callback.
    bool control(RWBuffer& ibuf, RWBuffer& obuf)
    {
        if constexpr (!std::is_same_v<Controller, tag::Empty>) {
            // Detect the type of the message by examining the element of the tuple returned by the parser.
            using Msg =
                std::tuple_element_t<2,
                                     std::invoke_result_t<decltype(&Parser::parse), Parser, const char*, std::size_t>>;

            // Detect the type of the command by examining the return type of the Controller commands method.
            using Cmd = typename std::invoke_result_t<decltype(&Controller::commands), Controller>::value_type;

            for (auto cmd : m_controller.commands()) {
                if constexpr (tag::is_control<std::remove_reference_t<decltype(m_handler)>>()) {
                    log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
                    for (auto msg : execute<Msg, Cmd>(cmd)) {
                        if (msg) {
                            if (!write(*msg, obuf)) {
                                log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                                    << false << std::endl;
                                return false;
                            }
                        } else {
                            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' '
                                << false << std::endl;
                            return false;
                        }
                    }
                }
            }
        }

        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << true << ' '
            << std::endl;
        return true;
    }

    // Invoke timeout callback.
    bool timeout(RWBuffer& ibuf, RWBuffer& obuf)
    {
        // Detect the type of the message by examining the element of the tuple returned by the parser.
        using Msg =
            std::tuple_element_t<2, std::invoke_result_t<decltype(&Parser::parse), Parser, const char*, std::size_t>>;

        if constexpr (tag::is_cyclical<std::remove_reference_t<decltype(m_handler)>>()) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
            for (auto msg : timeout<Msg>()) {
                if (msg) {
                    if (!write(*msg, obuf)) {
                        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << false << std::endl;
                        return false;
                    }
                } else {
                    log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << false << std::endl;
                    return false;
                }
            }
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << true << std::endl;
        }
        return true;
    }

private:
    // Invoke timer callback. It is a template function in case of pipeline modules
    template<typename MSG>
    auto timeout()
    {
        if constexpr (tag::is_pipeline<Handler>()) {
            return m_handler.template timeout<MSG>();
        } else {
            return m_handler.timeout();
        }
    }

    // Invoke control callback. It is a template function in case of pipeline modules
    template<typename MSG, typename CMD>
    auto execute(const CMD& cmd)
    {
        if constexpr (tag::is_pipeline<Handler>()) {
            return m_handler.template execute<MSG, CMD>(cmd);
        } else {
            return m_handler.execute(cmd);
        }
    }

    // Dispatch messages to the writer module. Code shared between notify and timeout.
    template<typename MSG>
    bool write(const MSG& message, RWBuffer& obuf)
    {
        auto [result, size] = m_writer.write(message, obuf.wpos(), obuf.wsize());
        obuf.produced(size);
        return result;
    }

    Parser     m_parser;
    Handler    m_handler;
    Writer     m_writer;
    Controller m_controller;
};

} // namespace paxsim::core

#endif
