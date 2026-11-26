#ifndef Common_Params_Store_dot_h
#define Common_Params_Store_dot_h

#include <cstddef>
#include <utility>

namespace Common::Params {

//---------------------------------------------------------------------------------------------------------------------
// Convenience class to collect all parameters.
//---------------------------------------------------------------------------------------------------------------------
template<typename... Parts>
class Store : public Parts...
{
public:
    Store() = default;

    template<typename... Args>
    explicit Store(Args&&... args)
      : Parts(std::forward<Args>(args)...)...
    {
    }

    template<typename Config>
    void init(const Config& config)
    {
        (Parts::init(config), ...);
    }

    void clear()
    {
        (Parts::clear(), ...);
    }
};

} // namespace Common::Params
#endif
