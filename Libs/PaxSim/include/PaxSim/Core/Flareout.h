#ifndef PaxSim_Core_Flareout_dot_h
#define PaxSim_Core_Flareout_dot_h

#include <PaxSim/Core/Types.h>
#include <PaxSim/Core/RWBuffer.h>
#include <PaxSim/Core/Streamlog.h>

#include <cassert>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// Flareout is a collection of message processing modules where each module receives the the same incoming message and
// produces zero or more outgoing messages. Timeout event produces zero or more outgoing messages. Flareout class does
// not handle IO events and has no ioevent callback.
//---------------------------------------------------------------------------------------------------------------------
template<typename... Ms>
class Flareout;

template<typename M, typename... Ms>
class Flareout<M, Ms...>
{
private:
    M               m_head;
    Flareout<Ms...> m_tail;
    timepoint       m_hexp = timepoint::min();

public:
    template<typename... Args>
    explicit Flareout(ORWBuffer& obuf, Args&&... args)
      : m_head(std::forward<Args>(args)...)
      , m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    template<typename... Args>
        requires std::is_constructible_v<M, ORWBuffer&, Args...>
    explicit Flareout(ORWBuffer& obuf, Args&&... args)
      : m_head(obuf, std::forward<Args>(args)...)
      , m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    // Message forwarding callback. Pass the message to every module in the collection.
    template<typename Msg, typename Next>
    bool put(Msg& msg, Next& next)
    {

        if constexpr (has_put<M, Msg, Next>) {
            if (m_head.put(msg, next)) {
                if constexpr (has_put<Flareout<Ms...>, Msg, Next>) {
                    return m_tail.put(msg, next);
                }
            }
        } else {
            if constexpr (has_put<Flareout<Ms...>, Msg, Next>) {
                return m_tail.put(msg, next);
            }
        }
        return true;
    }

    // Timeout callback. Invoke timeout callback on every module of the collection.
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_hexp) {

            if constexpr (has_timeout<M, Next>) {
                m_hexp = m_head.timeout(now, next);
            } else {
                m_hexp = timepoint::max();
            }
        }
        if constexpr (has_timeout<Flareout<Ms...>, Next>) {
            return std::min(m_tail.timeout(now, next), m_hexp);
        }
        return std::min(timepoint::max(), m_hexp);
    }

    // Init callback. Invoke init callback on every module of the collection.
    template<typename Next>
    void init(Next& next)
    {
        if constexpr (has_init<M, Next>) {
            m_head.init(next);
        }
        m_tail.init(next);
    }

    // Eval callback. Invoke eval callback on every module of the collection.
    template<typename Next>
    void eval(Next& next)
    {
        if constexpr (has_eval<M, Next>) {
            m_head.eval(next);
        }
        m_tail.eval(next);
    }
};

template<typename M>
class Flareout<M>
{
private:
    M         m_tail;
    timepoint m_texp = timepoint::min();

public:
    template<typename... Args>
    explicit Flareout(ORWBuffer& obuf, Args&&... args)
      : m_tail(std::forward<Args>(args)...)
    {
    }

    template<typename... Args>
        requires std::is_constructible_v<M, ORWBuffer&, Args...>
    explicit Flareout(ORWBuffer& obuf, Args&&... args)
      : m_tail(obuf, std::forward<Args>(args)...)
    {
    }

    // Message forwarding callback. Pass the message to the module in the collection.
    template<typename Msg, typename Next>
    bool put(Msg& msg, Next& next)
    {
        if constexpr (has_put<M, Msg, Next>) {
            return m_tail.put(msg, next);
        }
        return false;
    }

    // Timeout callback. Invoke timeout callback on the module of the collection.
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_texp) {
            if constexpr (has_timeout<M, Next>) {
                m_texp = m_tail.timeout(now, next);
            } else {
                m_texp = timepoint::max();
            }
        }
        return m_texp;
    }

    // Init callback. Invoke init callback on every module of the collection.
    template<typename Next>
    void init(Next& next)
    {
        if constexpr (has_init<M, Next>) {
            m_tail.init(next);
        }
    }

    // Eval callback. Invoke eval callback on every module of the collection.
    template<typename Next>
    void eval(Next& next)
    {
        if constexpr (has_eval<M, Next>) {
            m_tail.eval(next);
        }
    }
};

} // namespace PaxSim::Core

#endif
