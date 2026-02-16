#ifndef Context_Session_Session_dot_h
#define Context_Session_Session_dot_h

#include <string>
#include <cstddef>

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
