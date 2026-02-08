#ifndef Source_dot_h
#define Source_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Read the full sentence from the input buffer and pass it to the next module.
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
        std::string_view msg(m_ibuf->rpos(), m_ibuf->rsize());
        if (auto pos = msg.find_first_of(".!?"); pos != std::string_view::npos) {
            log << level::info << in << '[' << hexdump(m_ibuf->rpos(), pos + 1) << ']' << std::endl;
            std::string_view msg(m_ibuf->rpos(), pos + 1);
            m_ibuf->consumed(pos + 1);

            next.put(msg);
        }
    }

private:
    IRWBuffer* m_ibuf;
};

} // namespace Text
#endif
