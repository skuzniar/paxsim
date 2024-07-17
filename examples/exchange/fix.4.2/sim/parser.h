#ifndef Paxsim_Examples_Fix42_Sim_Parser_dot_h
#define Paxsim_Examples_Fix42_Sim_Parser_dot_h

#include "quickfix/Message.h"
#include "core/streamlog.h"
#include "types.h"

#include <string>
#include <regex>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol parser. Upon success returns number of bytes consumed and a message.
//---------------------------------------------------------------------------------------------------------------------
class Parser
{
    static constexpr const char* iam = "Parser    ";

public:
    template<typename Context>
    Parser(Context& context)
    {
    }

    std::tuple<bool, std::size_t, FIX::Message> parse(const char* ibuf, std::size_t size)
    {
        static std::regex rx("^8=(FIX[.0-9]*)[\x01]9=([0-9]+)[\x01]");

        std::cmatch match;
        std::regex_search(ibuf, ibuf + size, match, rx);

        if (match.size() == 3) {
            std::size_t offset = atol(match[2].str().c_str()) + match[0].length();
            if (offset < size) {
                static std::regex rx("10=([0-9]+)[\x01]");

                std::cmatch match;
                std::regex_search(ibuf + offset, ibuf + size, match, rx);
                if (match.size() == 2) {
                    auto length = offset + match[0].length();
                    log << level::info << in << iam << '[' << fixdump(ibuf, length) << ']' << std::endl;
                    return { true, length, std::string(ibuf, length) };
                }
            }
        }

        // TODO - Fail if not a partial message

        log << level::debug << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "size=" << size
            << std::endl;
        return { true, 0, {} };
    }
};

} // namespace fix42
#endif
