#ifndef Params_Generic_dot_h
#define Params_Generic_dot_h

#include "Common/Config.h"

#include <vector>
#include <cstddef>

namespace Params {

//---------------------------------------------------------------------------------------------------------------------
// Generic control parameters.
//---------------------------------------------------------------------------------------------------------------------
class Generic
{
public:
    using Config = Common::Config;

    using Selector  = Common::Config;
    using Qualifier = Common::Config;

    Generic() = default;

    auto begin() const
    {
        return parameters.begin();
    }
    auto end() const
    {
        return parameters.end();
    }

    std::size_t clear()
    {
        auto count = parameters.size();
        parameters.clear();
        return count;
    }

protected:
    std::size_t load(const Config& config)
    {
        std::size_t bcount = parameters.size();
        for (const auto& entry : config) {
            parameters.emplace_back(entry["Selector"], entry["Qualifier"]);
        }
        return parameters.size() - bcount;
    }

    std::vector<std::pair<Selector, Qualifier>> parameters;
};

} // namespace Params
#endif
