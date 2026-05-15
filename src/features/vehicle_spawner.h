#pragma once

#include <string>

namespace Features::VehicleSpawner {
    bool SpawnVehicle(const std::string& spawnQuery);
    const std::string& GetLastStatus();
}
