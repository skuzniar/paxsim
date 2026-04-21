#ifndef FIX_Modules_Factory_FIX42_Parser_dot_h
#define FIX_Modules_Factory_FIX42_Parser_dot_h

#include "FIX/Types.h"

namespace FIX::Modules::Factory::FIX42 {

//---------------------------------------------------------------------------------------------------------------------
// FIX 4.2 protocol message factory. Attempt to get complete message from the buffer.
//---------------------------------------------------------------------------------------------------------------------
struct Parser
{
    static std::pair<bool, const FIX::IMessage> message(const char* buff, std::size_t size)
    {
        FIX::IMessage imsg(buff, size);
        return { imsg.is_complete() && imsg.is_valid(), imsg };
    }
};

} // namespace FIX::Modules::Factory::FIX42
#endif
