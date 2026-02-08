#ifndef Config_dot_h
#define Config_dot_h

#include <sol/sol.hpp>
#include <cstddef>

namespace Text {

//---------------------------------------------------------------------------------------------------------------------
// Text protocol Config.
//---------------------------------------------------------------------------------------------------------------------
struct Config
{
    sol::state lua;

    Config(int argc, char* argv[])
    {
        lua.open_libraries(sol::lib::base);

        auto options = file(argv, argv + argc, "-cfg");
        if (!options.empty()) {
            lua.script_file(options);
        }
    }

    auto operator[](const char* option)
    {
        return lua[option];
    }

private:
    static std::string file(char** beg, char** end, const std::string& opt)
    {
        if (auto* itr = std::find(beg, end, opt); itr != end && std::next(itr) != end) {
            return *std::next(itr);
        }
        return {};
    }
};

} // namespace Text
#endif
