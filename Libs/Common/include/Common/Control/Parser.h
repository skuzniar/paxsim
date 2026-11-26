#ifndef Common_Control_Parser_dot_h
#define Common_Control_Parser_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace Common::Control {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Read the full line from the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
class Parser
{
public:
    using Config = Common::Config;

    template<typename Context>
    Parser(IRWBuffer& ibuf, Context& context)
      : m_ibuf(ibuf)
    {
    }

    template<typename Context>
    Parser(IRWBuffer& ibuf, const Config&, Context& context)
      : m_ibuf(ibuf)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        if (auto* pos = std::find(m_ibuf.rpos(), m_ibuf.rpos() + m_ibuf.rsize(), '\0'); pos != m_ibuf.rpos() + m_ibuf.rsize()) {
            const char* msg = m_ibuf.rpos();
            log << level::info << in << '[' << msg << ']' << std::endl;
            m_ibuf.rmove(std::distance(m_ibuf.rpos(), pos) + 1);
            next.put(msg);
        }
    }

private:
    IRWBuffer& m_ibuf;
};

} // namespace Common::Control
#endif
