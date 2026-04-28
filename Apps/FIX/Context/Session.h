#ifndef FIX_Context_Session_dot_h
#define FIX_Context_Session_dot_h

#include <string>
#include <cstddef>
#include <iostream>

namespace FIX::Context {

//---------------------------------------------------------------------------------------------------------------------
// Session context. Captures relevant configuration parameters.
//---------------------------------------------------------------------------------------------------------------------
struct Session
{
    // Session state
    enum class State
    {
        LogonSent,
        LogonWait,
        Normal,
        Recovery
    };

    friend [[cppgen::auto]] std::string to_string(State o)
    {
        switch (o) {
                // clang-format off
            case State::LogonSent: return "(LogonSent)"; break;
            case State::LogonWait: return "(LogonWait)"; break;
            case State::Normal:    return "(Normal)";    break;
            case State::Recovery:  return "(Recovery)";  break;
            default: return std::to_string(static_cast<std::underlying_type_t<State>>(o)) + "(Invalid State)"; break;
                // clang-format on
        };
    }

    friend [[cppgen::auto]] std::ostream& operator<<(std::ostream& s, State o)
    {
        switch (o) {
                // clang-format off
            case State::LogonSent: s << "(LogonSent)"; break;
            case State::LogonWait: s << "(LogonWait)"; break;
            case State::Normal:    s << "(Normal)";    break;
            case State::Recovery:  s << "(Recovery)";  break;
            default: s << std::to_string(static_cast<std::underlying_type_t<State>>(o)) + "(Invalid State)"; break;
                // clang-format on
        };
        return s;
    }

    template<typename Config>
    explicit Session(Config& config)
    {
        if (const auto& cfg = config["Session"]; cfg.valid()) {
            BeginString = cfg["BeginString"];

            TargetCompID = cfg["TargetCompID"];
            SenderCompID = cfg["SenderCompID"];

            UserName = cfg["UserName"];
            Password = cfg["Password"];

            HBInterval = cfg["HBInterval"];
        }
    }

    void reset()
    {
        ISequenceNumber = 1;
        OSequenceNumber = 1;
    }

    const std::string& beginString() const
    {
        return BeginString;
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
    int iSequence(int sequence)
    {
        return ISequenceNumber = sequence;
    }
    int oSequence() const
    {
        return OSequenceNumber;
    }
    int oSequence(int sequence)
    {
        return OSequenceNumber = sequence;
    }

    int iSequenceAdvance()
    {
        return ISequenceNumber++;
    }
    int oSequenceAdvance()
    {
        return OSequenceNumber++;
    }

    State state() const
    {
        return m_state;
    }
    void state(State s)
    {
        m_state = s;
    }

    State m_state = State::LogonWait;

    std::string BeginString;

    std::string TargetCompID;
    std::string SenderCompID;

    std::string UserName;
    std::string Password;

    int HBInterval = 30;

    int ISequenceNumber = 1;
    int OSequenceNumber = 1;
};

} // namespace FIX::Context
#endif
