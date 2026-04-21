#ifndef Format_dot_h
#define Format_dot_h

#include "PaxSim/Core/Streamlog.h"
#include "PaxSim/Core/Types.h"

#include <regex>

namespace Text {

using namespace PaxSim::Core;

using PaxSim::Core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Format incoming message and pass it to the next  module.
//---------------------------------------------------------------------------------------------------------------------
class Format1
{
public:
    template<typename Next>
    bool put(std::string_view msg, Next& next)
    {
        // Format the message by removing leading spaces, replacing new lines with spaces an upcasing first letter.
        auto fmt = std::string(msg);
        fmt      = fmt.substr(msg.find_first_not_of(" \t\n"));

        std::replace(fmt.begin(), fmt.end(), '\n', ' ');

        *fmt.begin() = std::toupper(*fmt.begin());

        log << level::info << vmark << '[' << hexdump(fmt.data(), fmt.size()) << ']' << std::endl;
        return next.put(fmt);
    }
};

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Format incoming message and pass it to the next  module.
//---------------------------------------------------------------------------------------------------------------------
class Format2
{
public:
    template<typename Next>
    bool put(std::string_view msg, Next& next)
    {
        // Format the message by removing leading spaces before puncuation marks.
        static const std::regex rgx_pattern("\\s*([.,\\?])");
        std::string             rtn;
        rtn.reserve(msg.size());
        std::regex_replace(std::back_insert_iterator<std::string>(rtn), msg.cbegin(), msg.cend(), rgx_pattern, "$1");

        log << level::info << vmark << '[' << hexdump(rtn.data(), rtn.size()) << ']' << std::endl;
        return next.put(rtn);
    }
};

} // namespace Text
#endif
