#ifndef Paxsim_Examples_Fix42_Ctl_Writer_dot_h
#define Paxsim_Examples_Fix42_Ctl_Writer_dot_h

#include "core/streamlog.h"
#include <json5cpp.h>

namespace fix42::ctl {

using namespace paxsim::core;

using paxsim::core::log;

using json = Json::Value;

//---------------------------------------------------------------------------------------------------------------------
// JSON protocol writer. Copy the message to the output buffer and add a newline.
//---------------------------------------------------------------------------------------------------------------------
class Writer
{
public:
    template<typename Context>
    Writer(Context& context)
    {
    }

    static std::pair<bool, std::size_t> write(const json& message, char* obuf, std::size_t size)
    {
        return { true, 0 };

        /*
        std::memcpy(obuf, message.data(), message.size());
        obuf[message.size()] = '\n';
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << '['
            << hexdump(obuf, message.size() + 1) << ']' << std::endl;

        log << level::info << out << '[' << hexdump(message.data(), message.size()) << ']' << std::endl;
        return { true, message.size() + 1 };
        */
    }
};

} // namespace fix42::ctl
#endif
