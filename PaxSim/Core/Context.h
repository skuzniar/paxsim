#ifndef PaxSim_Core_Context_dot_h
#define PaxSim_Core_Context_dot_h

#include <cstddef>
#include <utility>

namespace PaxSim::Core {

//---------------------------------------------------------------------------------------------------------------------
// Context pulls together a set of context modules into one class. Users have access to individual modules via base
// class reference.
//---------------------------------------------------------------------------------------------------------------------
template<typename... Ctxs>
class Context : public Ctxs...
{
public:
    Context() = default;

    template<typename... Args>
    explicit Context(Args&&... args)
      : Ctxs(std::forward<Args>(args)...)...
    {
    }
};

} // namespace PaxSim::Core
#endif
