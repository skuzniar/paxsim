#ifndef Paxsim_Examples_Telnet_Parser_dot_h
#define Paxsim_Examples_Telnet_Parser_dot_h

#include "context.h"

#include "paxsim/core/streamlog.h"
#include <string>

namespace text {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol parser. Upon success returns number of bytes consumed and a message.
//---------------------------------------------------------------------------------------------------------------------
class Parser
{
public:
    Parser(Context& context)
    {
    }

    std::tuple<bool, std::size_t, std::string> parse(const char* ibuf, std::size_t size)
    {
        auto pos = std::string_view(ibuf, size).find_first_of(".!?");
        if (pos != std::string_view::npos) {
            log << level::info << in << '[' << hexdump(ibuf, pos + 1) << ']' << std::endl;
            return { true, pos + 1, std::string(ibuf, pos + 1) };
        }
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "size=" << size << std::endl;
        return { true, 0, {} };
    }
};

} // namespace text
#endif
