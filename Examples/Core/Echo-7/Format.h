#ifndef Format_dot_h
#define Format_dot_h

#include "PaxSim/Core/Streamlog.h"

#include "Context.h"

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
    Format1(Context& context)
      : m_context(&context)
    {
        // Demonstrate the use of configuration options
        const auto& cfg = m_context->config()["Config"];

        int         dummy   = cfg["dummy"];
        std::string name    = cfg["name"];
        double      version = cfg["version"];

        std::cout << "Game Name: " << name << std::endl;
        std::cout << "Version: " << version << std::endl;
        std::cout << "Dummy: " << dummy << std::endl;
    }

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

private:
    Context* m_context;
};

//---------------------------------------------------------------------------------------------------------------------
// Text protocol handler. Format incoming message and pass it to the next  module.
//---------------------------------------------------------------------------------------------------------------------
class Format2
{
public:
    Format2(Context& context)
      : m_context(&context)
    {
    }

    template<typename Next>
    bool put(std::string_view msg, Next& next)
    {
        // Format the message by removing leading spaces before puncuation marks.
        static const std::regex rgx_pattern("\\s*([.,\\?])");
        std::string             rtn;
        rtn.reserve(msg.size());
        std::regex_replace(std::back_insert_iterator<std::string>(rtn), msg.cbegin(), msg.cend(), rgx_pattern, "$1");

        // Here we use the sentence count that has been updated elsewhere
        rtn = '[' + std::to_string(m_context->Counter) + ']' + rtn;
        log << level::info << vmark << '[' << hexdump(rtn.data(), rtn.size()) << ']' << std::endl;
        return next.put(rtn);
    }

private:
    Context* m_context;
};

} // namespace Text
#endif
