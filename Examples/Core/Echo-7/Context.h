#ifndef Context_dot_h
#define Context_dot_h

#include "Config.h"

#include <cstddef>

namespace Text {

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Context. Takes config class as a reference.
//---------------------------------------------------------------------------------------------------------------------
struct Context
{
    std::size_t   Counter = 0;
    const Config* m_cfg;

    explicit Context(Config& cfg)
      : m_cfg(&cfg)
    {
    }

    const Config& config() const
    {
        return *m_cfg;
    }
};

} // namespace Text
#endif
