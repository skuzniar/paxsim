#ifndef Common_Config_dot_h
#define Common_Config_dot_h

#include "PaxSim/Core/Types.h"

#include <sol/sol.hpp>
#include <cstddef>
#include <filesystem>
#include <iostream>

namespace Common {
//---------------------------------------------------------------------------------------------------------------------
// Configuration parser. Parses configuration file and makes options available to the caller.
//---------------------------------------------------------------------------------------------------------------------
struct Config
{
    using Table = sol::table;

    explicit Config(const std::filesystem::path& file)
    {
        if (!std::filesystem::is_regular_file(file)) {
            throw std::runtime_error(file.string() + " is not a regular file.");
        }

        lua.open_libraries(sol::lib::base);

        if (!file.empty()) {
            lua.script_file(file);
        }
        cfg = lua["Config"];
    }

    explicit Config(const std::string_view script)
    {
        lua.open_libraries(sol::lib::base);

        if (!script.empty()) {
            lua.safe_script(script);
        }
        cfg = lua["Config"];
    }

    auto operator[](std::string_view option) const
    {
        sol::table t = cfg;
        for (const auto& token : PaxSim::Core::tokenize(option, ".")) {
            if (sol::table tt = t[token]; tt.valid()) {
                t = tt;
            } else {
                throw std::runtime_error("Invalid configuration path: " + std::string(option) + " @" + std::string(token));
            }
        }
        return t;
    }

private:
    sol::state lua;
    sol::table cfg;
};

inline std::ostream&
operator<<(std::ostream& os, const Config::Table& tbl)
{
    os << '{';
    for (auto it = tbl.begin(); it != tbl.end(); ++it) {
        if (it != tbl.begin()) {
            os << ", ";
        }
        if ((*it).first.is<std::string>()) {
            os << (*it).first.as<std::string>();
        } else {
            os << "[" << (*it).first.as<std::string>() << "]";
        }
        os << " = ";
        if ((*it).second.is<sol::table>()) {
            os << (*it).second.as<sol::table>();
        } else {
            os << (*it).second.as<std::string>();
        }
    }
    return os << '}';
}

} // namespace Common

#endif
