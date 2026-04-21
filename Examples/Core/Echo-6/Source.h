#ifndef Source_dot_h
#define Source_dot_h

#include "PaxSim/Core/RWBuffer.h"
#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include "Context.h"

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Read the full sentence from the input buffer and pass it to the next module.
//---------------------------------------------------------------------------------------------------------------------
class Source
{
public:
    Source(IRWBuffer& ibuf, Context& context)
      : m_ibuf(&ibuf)
      , m_context(&context)
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

            // Here we update the sentence count
            m_context->Counter++;

            next.put(msg);
            m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(15);
        }
    }

    template<typename Next>
    timepoint timeout(timepoint now, Next& next)
    {
        if (std::chrono::steady_clock::now() >= m_expr) {
            std::string msg = "(Waiting for a complete sentence...)\n";
            log << level::info << status << '[' << hexdump(msg.data(), msg.size()) << ']' << std::endl;
            next.put(msg);
        }
        return m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(15);
    }

private:
    IRWBuffer* m_ibuf;
    Context*   m_context;

    timepoint m_expr = std::chrono::steady_clock::now() + std::chrono::seconds(15);
};

} // namespace Text
#endif
