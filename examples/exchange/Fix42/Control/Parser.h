#ifndef Paxsim_Examples_Fix42_Ctl_Parser_dot_h
#define Paxsim_Examples_Fix42_Ctl_Parser_dot_h

#include "core/streamlog.h"
#include <json5cpp.h>
#include <sstream>

namespace fix42::ctl {

using namespace paxsim::core;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// JSON parser. Returns JSON object if successful.
//---------------------------------------------------------------------------------------------------------------------
class Parser
{
public:
    template<typename Context>
    Parser(Context& context)
    {
    }

    std::tuple<bool, std::size_t, json> parse(const char* ibuf, std::size_t size)
    {
        if (size > 0) {
            std::istringstream is(std::string(ibuf, size));

            json        command;
            std::string error;
            if (Json5::parse(is, command, &error)) {
                log << level::debug << in << '[' << command << ']' << std::endl;
                return { true, size, command };
            }
            log << level::error << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "Error parsing command input."
                << " Input: " << std::string_view(ibuf, size) << " Error: " << error << std::endl;
        }
        return { true, 0, {} };
    }
};

} // namespace fix42::ctl
#endif
