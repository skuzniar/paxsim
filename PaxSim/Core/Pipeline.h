#ifndef PaxSim_Core_Pipeline_dot_h
#define PaxSim_Core_Pipeline_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/RWBuffer.h>
#include <PaxSim/Core/Streamlog.h>

#include <type_traits>
#include <cassert>

namespace PaxSim::Core {

//----------------------------------------------------------------------------------------------------------------------
// Pipeline class handles IO and timer events. It is a sequence of message processing modules. Each module consumes
// the incoming message and produces zero or more outgoing messages. These messages are passed to the next module in the
// sequence. Timeout events produce zero or more messages which are passed to the next module.
// When Pipeline is constructed using input and/or output RWBuffer, the first module and/or the last module play the
// role of source and target modules, respectively.
//----------------------------------------------------------------------------------------------------------------------
template<typename... Ms>
class Pipeline;

template<typename M, typename... Ms>
class Pipeline<M, Ms...>
{
private:
    M               m_head;
    Pipeline<Ms...> m_tail;
    timepoint       m_hexp = timepoint::min();

public:
    // Pass the input buffer to the first module and the output buffer to the last module of the pipeline.
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf)
      : m_head(ibuf)
      , m_tail(obuf)
    {
    }

    template<typename Context>
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf, Context& context)
      : m_head(ibuf, context)
      , m_tail(obuf, context)
    {
    }

    // Pass the input buffer to the first module of the pipeline.
    explicit Pipeline(IRWBuffer& ibuf)
      : m_head(ibuf)
    {
    }

    template<typename Context>
    Pipeline(IRWBuffer& ibuf, Context& context)
      : m_head(ibuf, context)
    {
    }

    // Pass the output buffer down the pipeline until it reaches the last module.
    explicit Pipeline(ORWBuffer& obuf)
      : m_tail(obuf)
    {
    }

    template<typename Context>
    Pipeline(ORWBuffer& obuf, Context& context)
      : m_head(context)
      , m_tail(obuf, context)
    {
    }

    // IO callback. Tell the head module to get a new message and forward the result to the tail.
    void ioevent()
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        m_head.ioevent(m_tail);
    }

    // Message forwarding callback. Accept the message and pass it to the tail module.
    template<typename Msg>
    bool put(Msg&& msg)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        return m_head.put(std::forward<Msg>(msg), m_tail);
    }

    // Timeout callback. Invoke timeout callback on the head and the rest of the pipeline.
    timepoint timeout(timepoint now)
    {
        if (now >= m_hexp) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;

            if constexpr (has_timeout_method<M, Pipeline<Ms...>>) {
                m_hexp = m_head.timeout(now, m_tail);
            } else {
                m_hexp = timepoint::max();
            }
        }
        return std::min(m_tail.timeout(now), m_hexp);
    }
};

template<typename M>
class Pipeline<M>
{
private:
    M         m_tail;
    timepoint m_texp = timepoint::min();

public:
    // Pass the input buffer and the output buffer to the module of the pipeline.
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf)
      : m_tail(ibuf, obuf)
    {
    }

    template<typename Context>
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf, Context& context)
      : m_tail(ibuf, obuf, context)
    {
    }

    // Pass the output buffer to the last module of the pipeline.
    explicit Pipeline(ORWBuffer& obuf)
      : m_tail(obuf)
    {
    }

    template<typename Context>
    Pipeline(ORWBuffer& obuf, Context& context)
      : m_tail(obuf, context)
    {
    }

    // IO callback. Forward IO callback to the tail module.
    void ioevent()
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        m_tail.ioevent();
    }

    // Message forwarding callback. Accept the message and pass it to the tail module.
    template<typename Msg>
    bool put(Msg&& msg)
    {
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
        return m_tail.put(std::forward<Msg>(msg));
    }

    // Timeout callback. Invoke timeout callback on the tail.
    timepoint timeout(timepoint now)
    {
        if (now >= m_texp) {
            log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << std::endl;
            if constexpr (has_timeout<M, timepoint, timepoint>) {
                m_texp = m_tail.timeout(now);
            } else {
                m_texp = timepoint::max();
            }
        }
        return m_texp;
    }
};

} // namespace PaxSim::Core

#endif
