#include "config.h"
#include <fstream>
#include <sstream>

namespace Config {
    std::unordered_map<std::string, Setting> settings;

    void Load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                Setting setting;
                setting.name = key;
                setting.enabled = (value == "1" || value == "true");
                setting.value = std::stof(value);
                settings[key] = setting;
            }
        }
    }

    void Save(const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) return;

        for (const auto& [key, setting] : settings) {
            file << key << "=" << (setting.enabled ? "1" : "0") << "\n";
        }
    }
}
