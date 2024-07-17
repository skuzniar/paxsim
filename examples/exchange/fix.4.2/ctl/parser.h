#ifndef Paxsim_Examples_Fix42_Ctl_Parser_dot_h
#define Paxsim_Examples_Fix42_Ctl_Parser_dot_h

#include "core/streamlog.h"
#include <nlohmann/json.hpp>

namespace fix42::ctl {

using namespace paxsim::core;

using paxsim::core::log;

using json = nlohmann::json;

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
        if (json::accept(ibuf, ibuf + size)) {
            auto json = json::parse(ibuf, ibuf + size);
            if (!json.empty()) {
                log << level::info << in << iam << '[' << json.dump() << ']' << std::endl;
                return { true, size, json };
            }
        }
        log << level::debug << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << "size=" << size << std::endl;
        return { true, 0, {} };
    }
};

} // namespace fix42::ctl
#endif
