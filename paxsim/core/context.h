#ifndef Paxsim_Core_Context_dot_h
#define Paxsim_Core_Context_dot_h

#include <cstddef>
#include <utility>

namespace paxsim::core {

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
    Context(Args&&... args)
      : Ctxs(std::forward<Args>(args)...)...
    {
    }
};

} // namespace paxsim::core
#endif
