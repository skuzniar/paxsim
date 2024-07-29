#ifndef Paxsim_Examples_Fix42_Ctl_Parser_dot_h
#define Paxsim_Examples_Fix42_Ctl_Parser_dot_h

#include "core/streamlog.h"
#include <json5cpp.h>
#include <sstream>
#include <regex>

namespace Fix42::Control {

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
            std::istringstream is(quoteit(std::string(ibuf, size)));

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

private:
    // This is a workaround to accommodate clients that do not send a properly formatted JSON message.
    static std::string quoteit(std::string s)
    {
        // Match JSON style Key: Value pair where neither has quotes
        static std::regex rex(R"((\s*[\w]+)(:\s*)([\w\n\s.]+))");

        std::string copy;
        std::string sufx;

        std::smatch m;
        while (std::regex_search(s, m, rex)) {
            copy += m.prefix();
            copy += m[1];
            copy += m[2];
            copy += quoteit(m[1], m[3]);
            s = sufx = m.suffix();
        }
        copy += sufx;
        return copy;
    }

    static std::string quoteit(const std::string& k, const std::string& v)
    {
        if (k == "Id" or std::any_of(v.begin(), v.end(), isalpha)) {
            return '"' + v + '"';
        }
        return v;
    }
};

} // namespace Fix42::Control
#endif
