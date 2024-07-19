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
    static constexpr const char* iam = "Parser    ";

public:
    template<typename Context>
    Parser(Context& context)
    {
    }

    std::tuple<bool, std::size_t, json> parse(const char* ibuf, std::size_t size)
    {
        std::istringstream is(ibuf, size);

        json        cmd;
        std::string err;
        if (Json5::parse(is, cmd, &err)) {
            log << level::info << in << iam << '[' << cmd << ']' << std::endl;
            return { true, size, cmd };
        }
        log << level::error << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "Error parsing command input."
            << std::endl;
        return { true, 0, {} };
    }
};

} // namespace fix42::ctl
#endif
