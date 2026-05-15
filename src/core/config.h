#pragma once

#include <string>
#include <unordered_map>

namespace Config {
    struct Setting {
        std::string name;
        bool enabled;
        float value;
    };
    
    void Load(const std::string& path);
    void Save(const std::string& path);
    
    // Feature settings
    extern std::unordered_map<std::string, Setting> settings;
}
