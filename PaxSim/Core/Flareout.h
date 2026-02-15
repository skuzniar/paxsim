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
    Flareout() = default;

    template<typename Context>
    explicit Flareout(Context& context)
      : m_head(context)
      , m_tail(context)
    {
    }

    // Message forwarding callback. Pass the message to every module in the collection.
    template<typename Msg, typename Next>
    bool put(Msg& msg, Next& next)
    {
        log << level::trace << ts << here << ' ' << __func__ << std::endl;

        if constexpr (has_put_method<M, Msg, Next>) {
            log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
            if (m_head.put(msg, next)) {
                if constexpr (has_put_method<Flareout<Ms...>, Msg, Next>) {
                    log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
                    return m_tail.put(msg, next);
                }
            }
        } else {
            if constexpr (has_put_method<Flareout<Ms...>, Msg, Next>) {
                log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
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
            log << level::trace << ts << here << ' ' << __func__ << std::endl;

            if constexpr (has_timeout_method<M, Next>) {
                log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
                m_hexp = m_head.timeout(now, next);
            } else {
                m_hexp = timepoint::max();
            }
        }
        return std::min(m_tail.timeout(now, next), m_hexp);
    }
};

template<typename M>
class Flareout<M>
{
private:
    M         m_tail;
    timepoint m_texp = timepoint::min();

public:
    Flareout() = default;

    template<typename Context>
    explicit Flareout(Context& context)
      : m_tail(context)
    {
    }

    // Message forwarding callback. Pass the message to the module in the collection.
    template<typename Msg, typename Next>
    bool put(Msg& msg, Next& next)
    {
        log << level::trace << ts << here << ' ' << __func__ << std::endl;
        if constexpr (has_put_method<M, Msg, Next>) {
            log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
            return m_tail.put(msg, next);
        }
        return false;
    }

    // Timeout callback. Invoke timeout callback on the module of the collection.
    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (now >= m_texp) {
            log << level::trace << ts << here << ' ' << __func__ << std::endl;
            if constexpr (has_timeout_method<M, Next>) {
                log << level::debug << ts << here << ' ' << "has" << ' ' << __func__ << std::endl;
                m_texp = m_tail.timeout(now, next);
            } else {
                m_texp = timepoint::max();
            }
        }
        return m_texp;
    }
};

} // namespace PaxSim::Core

#endif
