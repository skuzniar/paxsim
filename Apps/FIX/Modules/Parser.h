#ifndef FIX_Modules_Parser_dot_h
#define FIX_Modules_Parser_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace FIX::Modules {

using namespace Common;
using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Read full message from the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Parser
{
public:
    template<typename Context>
    Parser(IRWBuffer& ibuf, const Config& config, Context& context)
      : m_ibuf(&ibuf)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        if (const auto& [valid, msg] = m_factory.message(m_ibuf->rpos(), m_ibuf->rsize()); valid) {
            log << level::info << in << '[' << msg << ']' << std::endl;
            m_ibuf->rmove(msg.size());
            next.put(msg);
        }
    }

private:
    IRWBuffer* m_ibuf;
    Factory    m_factory;
};

} // namespace FIX::Modules
#endif
