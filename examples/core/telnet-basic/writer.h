#ifndef Paxsim_Examples_Telnet_Writer_dot_h
#define Paxsim_Examples_Telnet_Writer_dot_h

#include "paxsim/core/streamlog.h"
#include <string>
#include <cassert>

namespace text {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol writer. Copy the message to the output buffer and add a newline.
//---------------------------------------------------------------------------------------------------------------------
class Writer
{
public:
    static std::pair<bool, std::size_t> write(const std::string& message, char* obuf, std::size_t size)
    {
        assert(message.size() + 1 < size);

        std::memcpy(obuf, message.data(), message.size());
        obuf[message.size()] = '\n';
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << '['
            << hexdump(obuf, message.size() + 1) << ']' << std::endl;

        log << level::info << out << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { true, message.size() + 1 };
    }
};

} // namespace text
#endif
