#ifndef Common_Config_dot_h
#define Common_Config_dot_h

#include "PaxSim/Core/Types.h"

#include <az/json/Reader.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Common {
//---------------------------------------------------------------------------------------------------------------------
// Configuration parser. Parses configuration file and makes options available to the caller.
//---------------------------------------------------------------------------------------------------------------------
class Config
{
public:
    explicit Config(const std::filesystem::path& file)
    {
        if (!std::filesystem::is_regular_file(file)) {
            throw std::runtime_error(file.string() + " is not a regular file.");
        }

        std::ifstream ifs(file);
        if (!ifs) {
            throw std::runtime_error("Unable to read " + file.string() + ".");
        }

        try {
            az::json::Reader(m_json).strictly().parse(ifs);
        } catch (const az::json::Error& e) {
            std::cout << "Error parising " << file << ". " << e.what() << " line: " << e.line() << " column: " << e.column() << '.' << std::endl;
        }
    }

    explicit Config(az::json::Value value)
      : m_json(value)
    {
    }

    Config operator[](std::string_view option) const
    {
        const az::json::Value* json = &m_json;
        for (const auto& token : PaxSim::Core::tokenize(option, ".")) {
            if (std::string stoken(token); json->has(stoken)) {
                json = &json->operator[](stoken);
            } else {
                throw std::runtime_error("Invalid configuration path: " + std::string(option) + " @" + stoken);
            }
        }
        return Config(*json);
    }

    bool operator()(std::string_view option) const
    {
        try {
            this->operator[](option);
        } catch (...) {
            return false;
        }
        return true;
    }

    explicit operator int() const
    {
        return m_json.operator int();
    }
    explicit operator double() const
    {
        return m_json.operator double();
    }
    explicit operator std::string() const
    {
        return m_json.operator std::string();
    }

    auto begin() const
    {
        return m_json.begin();
    }
    auto end() const
    {
        return m_json.end();
    }

private:
    az::json::Value m_json;
};

} // namespace Common

#endif
