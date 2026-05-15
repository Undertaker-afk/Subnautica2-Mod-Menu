#pragma once

#include <string>

namespace Features::Items {
    bool GiveItem(const std::string& itemName, int quantity);
    const std::string& GetLastStatus();
}
