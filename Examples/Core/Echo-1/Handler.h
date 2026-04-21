#ifndef Handler_dot_h
#define Handler_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Read the content of the input buffer and pass it to the output buffer.
//---------------------------------------------------------------------------------------------------------------------
class Handler
{
public:
    Handler(IRWBuffer& ibuf, ORWBuffer& obuf)
      : m_ibuf(&ibuf)
      , m_obuf(&obuf)
    {
    }

    void ioevent()
    {
        log << level::info << in << '[' << hexdump(m_ibuf->rpos(), m_ibuf->rsize()) << ']' << std::endl;
        std::string_view msg(m_ibuf->rpos(), m_ibuf->rsize());
        m_ibuf->consumed(m_ibuf->rsize());

        std::memcpy(m_obuf->wpos(), msg.data(), msg.size());
        m_obuf->produced(msg.size());
        log << level::info << out << '[' << hexdump(m_obuf->rpos(), m_obuf->rsize()) << ']' << std::endl;
    }

private:
    IRWBuffer* m_ibuf;
    ORWBuffer* m_obuf;
};

} // namespace Text
#endif
