#ifndef Target_dot_h
#define Target_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include "Context.h"

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Accept the message from the previous module and pass it to the output buffer.
//---------------------------------------------------------------------------------------------------------------------
class Target
{
public:
    Target(ORWBuffer& obuf, Context& context)
      : m_obuf(&obuf)
      , m_context(&context)
    {
    }

    bool put(std::string_view msg)
    {
        std::memcpy(m_obuf->wpos(), msg.data(), msg.size());
        m_obuf->produced(msg.size());
        log << level::info << out << '[' << hexdump(msg.data(), msg.size()) << ']' << std::endl;
        return false;
    }

private:
    ORWBuffer* m_obuf;
    Context*   m_context;
};

} // namespace Text
#endif
