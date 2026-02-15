#ifndef Context_Session_Session_dot_h
#define Context_Session_Session_dot_h

#include <string>
#include <cstddef>
#include <iostream>

namespace Fix::Context {

//---------------------------------------------------------------------------------------------------------------------
// Session context. Captures relevant configuration parameters.
//---------------------------------------------------------------------------------------------------------------------
struct Session
{
    // Session state
    enum class State
    {
        Startup,
        Normal,
        Recovery
    };

    friend  [[cppgen::auto]] std::string to_string(State o)
    {
        switch(o)
        {
            // clang-format off
            case State::Startup:  return "Startup";  break;
            case State::Normal:   return "Normal";   break;
            case State::Recovery: return "Recovery"; break;
            default: return std::to_string(static_cast<std::underlying_type_t<State>>(o)) + "Invalid State"; break;
            // clang-format on
        };
    }

    friend  [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, State o)
    {
        switch(o)
        {
            // clang-format off
            case State::Startup:  s << "Startup";  break;
            case State::Normal:   s << "Normal";   break;
            case State::Recovery: s << "Recovery"; break;
            default: s << std::to_string(static_cast<std::underlying_type_t<State>>(o)) + "Invalid State"; break;
            // clang-format on
        };
        return s;
    }

    template<typename Config>
    explicit Session(Config& config)
    {
        const auto& cfg = config["Session"];

        TargetCompID = cfg["TargetCompID"];
        SenderCompID = cfg["SenderCompID"];
        HBInterval   = cfg["HBInterval"];
    }

    void reset()
    {
        ISequenceNumber = 1;
        OSequenceNumber = 1;
    }

    const std::string& senderCompID() const
    {
        return SenderCompID;
    }

    const std::string& targetCompID() const
    {
        return TargetCompID;
    }

    int hbInterval() const
    {
        return HBInterval;
    }

    int iSequence() const
    {
        return ISequenceNumber;
    }
    int oSequence() const
    {
        return OSequenceNumber;
    }

    int nextISequence()
    {
        return ISequenceNumber++;
    }
    int nextOSequence()
    {
        return OSequenceNumber++;
    }

    void iAdvance()
    {
        ++ISequenceNumber;
    }
    void oAdvance()
    {
        ++OSequenceNumber;
    }

    State state() const
    {
        return m_state;
    }
    void state(State s)
    {
        m_state = s;
    }

    State m_state = State::Startup;

    std::string TargetCompID;
    std::string SenderCompID;

    int HBInterval = 30;

    int ISequenceNumber = 1;
    int OSequenceNumber = 1;
};

} // namespace Fix::Context
#endif
