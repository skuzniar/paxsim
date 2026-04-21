#ifndef OUCH_Modules_Factory_OUCH50_Parser_dot_h
#define OUCH_Modules_Factory_OUCH50_Parser_dot_h

#include "OUCH/OUCH50/PacketHeader.h"

namespace OUCH::Modules::Factory::OUCH50 {

//---------------------------------------------------------------------------------------------------------------------
// OUCH 50 protocol message factory. Attempt to get complete message from the buffer.
//---------------------------------------------------------------------------------------------------------------------
struct Parser
{
    using PacketHeader    = OUCH::OUCH50::PacketHeader;
    using SequencedData   = OUCH::OUCH50::SequencedData;
    using UnsequencedData = OUCH::OUCH50::UnsequencedData;

    static std::pair<bool, const PacketHeader&> message(const char* buff, std::size_t size)
    {
        const auto* header = reinterpret_cast<const PacketHeader*>(buff);
        if (size >= sizeof(PacketHeader)) {
            std::size_t length = header->size();
            if (size >= length) {
                return { true, *header };
            }
        }
        return { false, *header };
    }
};

} // namespace OUCH::Modules::Factory::OUCH50
#endif
