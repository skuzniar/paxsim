#ifndef OUCH_Modules_Parser_dot_h
#define OUCH_Modules_Parser_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Common/Config.h"

namespace OUCH::Modules {

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
    using SequencedData   = Factory::SequencedData;
    using UnsequencedData = Factory::UnsequencedData;

    template<typename Context>
    Parser(IRWBuffer& ibuf, const Config& config, Context& context)
      : m_ibuf(ibuf)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        if (const auto& [valid, msg] = m_factory.message(m_ibuf.rpos(), m_ibuf.rsize()); valid) {

            switch (msg.type) {
                case SequencedData::Type:
                    log << level::info << in << '[' << reinterpret_cast<const SequencedData&>(msg) << ']' << std::endl;
                    break;
                case UnsequencedData::Type:
                    log << level::info << in << '[' << reinterpret_cast<const UnsequencedData&>(msg) << ']' << std::endl;
                    break;
                default:
                    log << level::info << in << '[' << msg << ']' << std::endl;
                    break;
            }
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
