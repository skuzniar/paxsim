#ifndef Source_dot_h
#define Source_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Read the content of the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
class Source
{
public:
    Source(IRWBuffer& ibuf)
      : m_ibuf(&ibuf)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        log << level::info << in << '[' << hexdump(m_ibuf->rpos(), m_ibuf->rsize()) << ']' << std::endl;
        std::string_view msg(m_ibuf->rpos(), m_ibuf->rsize());
        m_ibuf->consumed(m_ibuf->rsize());

        next.put(msg);
    }

private:
    IRWBuffer* m_ibuf;
};

} // namespace Text
#endif
