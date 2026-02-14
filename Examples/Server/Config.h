#ifndef Core_Config_dot_h
#define Core_Config_dot_h

#include "PaxSim/Core/Types.h"

#include <sol/sol.hpp>
#include <cstddef>

//---------------------------------------------------------------------------------------------------------------------
// Fix protocol Config. Parses configuration file and makes options available to the caller.
//---------------------------------------------------------------------------------------------------------------------
struct Config
{
    explicit Config(const std::string& file)
    {
        lua.open_libraries(sol::lib::base);

        if (!file.empty()) {
            lua.script_file(file);
        }
        cfg = lua["Config"];
    }

    auto operator[](std::string_view option) const
    {
        sol::table t = cfg;
        for (const auto& token : PaxSim::Core::tokenize(option, ".")) {
            t = t[token];
        }
        return t;
    }

private:
    sol::state lua;
    sol::table cfg;
};

#endif
