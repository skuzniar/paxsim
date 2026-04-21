#ifndef PaxSim_Core_Pipeline_dot_h
#define PaxSim_Core_Pipeline_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/RWBuffer.h>
#include <PaxSim/Core/Streamlog.h>

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
    template<typename... Args>
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf, Args&&... args)
      : m_head(ibuf, std::forward<Args>(args)...)
      , m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    // Pass the input buffer to the first module of the pipeline.
    template<typename... Args>
    explicit Pipeline(IRWBuffer& ibuf, Args&&... args)
      : m_head(ibuf, std::forward<Args>(args)...)
    {
    }

    // Pass the output buffer down the pipeline until it reaches the last module.
    template<typename... Args>
    explicit Pipeline(ORWBuffer& obuf, Args&&... args)
      : m_head(std::forward<Args>(args)...)
      , m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    template<typename... Args>
        requires std::is_constructible_v<M, ORWBuffer&, Args...>
    explicit Pipeline(ORWBuffer& obuf, Args&&... args)
      : m_head(obuf, std::forward<Args>(args)...)
      , m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    // IO callback. Tell the head module to get a new message and forward the result to the tail.
    void ioevent()
    {
        m_head.ioevent(m_tail);
    }

    // Message forwarding callback. Accept the message and pass it to the tail module.
    template<typename Msg>
    bool put(Msg&& msg)
    {
        return m_head.put(std::forward<Msg>(msg), m_tail);
    }

    // Timeout callback. Invoke timeout callback on the head and the rest of the pipeline.
    timepoint timeout(timepoint now)
    {
        if (now >= m_hexp) {

            if constexpr (has_timeout<M, Pipeline<Ms...>>) {
                m_hexp = m_head.timeout(now, m_tail);
            } else {
                m_hexp = timepoint::max();
            }
        }
        return std::min(m_tail.timeout(now), m_hexp);
    }

    // Init callback. Invoke init callback on the head and the rest of the pipeline.
    void init()
    {
        if constexpr (has_init<M, Pipeline<Ms...>>) {
            m_head.init(m_tail);
        }
        m_tail.init();
    }

    // Eval callback. Invoke eval callback on the head and the rest of the pipeline.
    void eval()
    {
        if constexpr (has_eval<M, Pipeline<Ms...>>) {
            m_head.eval(m_tail);
        }
        m_tail.eval();
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
    template<typename... Args>
    Pipeline(IRWBuffer& ibuf, ORWBuffer& obuf, Args&&... args)
      : m_tail(ibuf, obuf, std::forward<Args>(args)...)
    {
    }

    // Pass the output buffer to the last module of the pipeline, if needed.
    template<typename... Args>
    explicit Pipeline(ORWBuffer& obuf, Args&&... args)
      : m_tail(std::forward<Args>(args)...)
    {
    }

    template<typename... Args>
        requires std::is_constructible_v<M, ORWBuffer&, Args...>
    explicit Pipeline(ORWBuffer& obuf, Args&&... args)
      : m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    // IO callback. Forward IO callback to the tail module.
    void ioevent()
    {
        m_tail.ioevent();
    }

    // Message forwarding callback. Accept the message and pass it to the tail module.
    template<typename Msg>
    bool put(Msg&& msg)
    {
        return m_tail.put(std::forward<Msg>(msg));
    }

    // Timeout callback. Invoke timeout callback on the tail.
    timepoint timeout(timepoint now)
    {
        if (now >= m_texp) {
            if constexpr (has_timeout<M>) {
                m_texp = m_tail.timeout(now);
            } else {
                m_texp = timepoint::max();
            }
        }
        return m_texp;
    }

    // Init callback. Invoke init callback on the tail of the pipeline.
    void init()
    {
        if constexpr (has_init<M>) {
            m_tail.init();
        }
    }

    // Eval callback. Invoke eval callback on the tail of the pipeline.
    void eval()
    {
        if constexpr (has_eval<M>) {
            m_tail.eval();
        }
    }
};

} // namespace PaxSim::Core

#endif
