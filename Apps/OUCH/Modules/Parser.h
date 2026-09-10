#ifndef OUCH_Modules_Parser_dot_h
#define OUCH_Modules_Parser_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace OUCH::Modules {

using namespace PaxSim::Core;
using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Read full message from the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
template<typename Factory>
class Parser
{
public:
    using Config = Common::Config;

    template<typename Context>
    Parser(IRWBuffer& ibuf, Context& context)
      : m_ibuf(ibuf)
      , m_factory(context)
    {
    }

    template<typename Context>
    Parser(IRWBuffer& ibuf, const Config&, Context& context)
      : m_ibuf(ibuf)
      , m_factory(context)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        if (const auto& [valid, msg] = m_factory.message(m_ibuf.rpos(), m_ibuf.rsize()); valid) {
            log << level::info << in << '[' << msg << ']' << std::endl;
            m_ibuf.rmove(msg.size());
            next.put(msg);
        }
    }

private:
    IRWBuffer& m_ibuf;
    Factory    m_factory;
};

} // namespace OUCH::Modules
#endif
