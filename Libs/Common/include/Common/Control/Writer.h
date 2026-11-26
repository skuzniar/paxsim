#ifndef Common_Control_Writer_dot_h
#define Common_Control_Writer_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace Common::Control {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Accept the message from the previous module and pass it to the output buffer.
//---------------------------------------------------------------------------------------------------------------------
class Writer
{
public:
    using Config = Common::Config;

    template<typename Context>
    Writer(ORWBuffer& obuf, Context& context)
      : m_obuf(obuf)
    {
    }

    template<typename Context>
    Writer(ORWBuffer& obuf, const Config&, Context& context)
      : m_obuf(obuf)
    {
    }

    bool put(std::string_view msg)
    {
        std::memcpy(m_obuf.wpos(), msg.data(), msg.size());
        m_obuf.wmove(msg.size());
        log << level::info << out << '[' << msg << ']' << std::endl;
        return false;
    }

private:
    ORWBuffer& m_obuf;
};

} // namespace Common::Control
#endif
