#ifndef Modules_Parser_Parser_dot_h
#define Modules_Parser_Parser_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"

#include "Utils.h"

#include "quickfix/Message.h"

#include <regex>

namespace Fix::Modules::Parser {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// FIX protocol parser. Read full FIX message from the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
class Parser
{
public:
    template<typename Context>
    Parser(IRWBuffer& ibuf, Context& context)
      : m_ibuf(&ibuf)
    {
    }

    template<typename Next>
    void ioevent(Next& next)
    {
        static std::regex rx("^8=(FIX[.0-9]*)[\x01]9=([0-9]+)[\x01]");

        std::cmatch match;
        std::regex_search(m_ibuf->rpos(), m_ibuf->rpos() + m_ibuf->rsize(), match, rx);

        if (match.size() == 3) {
            std::size_t offset = atol(match[2].str().c_str()) + match[0].length();
            if (offset < m_ibuf->rsize()) {
                static std::regex rx("10=([0-9]+)[\x01]");

                std::cmatch match;
                std::regex_search(m_ibuf->rpos() + offset, m_ibuf->rpos() + m_ibuf->rsize(), match, rx);
                if (match.size() == 2) {
                    auto length = offset + match[0].length();
                    log << level::info << in << '[' << fixdump(m_ibuf->rpos(), length) << ']' << std::endl;
                    FIX::Message msg(std::string(m_ibuf->rpos(), length), true);
                    m_ibuf->consumed(length);
                    next.put(msg);
                }
            }
        }
        // TODO - Fail if not a partial message
        log << level::debug << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "size=" << m_ibuf->rsize() << std::endl;
    }

private:
    IRWBuffer* m_ibuf;
};

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
} // namespace Fix::Modules::Parser
#endif
