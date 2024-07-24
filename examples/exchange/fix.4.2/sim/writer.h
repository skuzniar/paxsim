#ifndef Paxsim_Examples_Fix42_Sim_Writer_dot_h
#define Paxsim_Examples_Fix42_Sim_Writer_dot_h

#include "quickfix/Message.h"
#include "core/streamlog.h"
#include "types.h"

#include <string>
#include <cassert>

namespace fix42::sim {

using namespace paxsim::core;

using paxsim::core::log;

//---------------------------------------------------------------------------------------------------------------------
// Text protocol writer. Copy the message to the output buffer and add a newline.
//---------------------------------------------------------------------------------------------------------------------
class Writer
{
public:
    template<typename Context>
    Writer(Context& context)
    {
    }

    static std::pair<bool, std::size_t> write(const FIX::Message& message, char* obuf, std::size_t size)
    {
        auto string = message.toString();
        assert(string.size() < size);

        std::memcpy(obuf, string.data(), string.size());
        log << level::trace << ts << ' ' << _file_ << ':' << _line_ << ' ' << __func__ << ' ' << '[' << hexdump(obuf, string.size())
            << ']' << std::endl;

        log << level::info << out << '[' << fixdump(string) << ']' << std::endl;
        return { true, string.size() };
    }
};

} // namespace fix42::sim
#endif
