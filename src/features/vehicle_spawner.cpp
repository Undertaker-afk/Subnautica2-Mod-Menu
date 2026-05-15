#include "vehicle_spawner.h"
#include "../core/sdk.h"
#include "../utils/helpers.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"
#include <algorithm>
#include <cctype>

namespace Features::VehicleSpawner {
    namespace {
        std::string g_lastStatus = "Ready.";

        std::string Trim(std::string value) {
            auto notSpace = [](unsigned char ch) {
                return !std::isspace(ch);
            };

            value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
            value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
            return value;
        }

        bool ContainsDigit(const std::string& value) {
            return std::any_of(value.begin(), value.end(), [](unsigned char ch) {
                return std::isdigit(ch) != 0;
            });
        }

        void SetStatus(std::string status) {
            g_lastStatus = std::move(status);
            Utils::Helpers::Log("[ModMenu][Vehicles] %s", g_lastStatus.c_str());
        }
    }

    bool SpawnVehicle(const std::string& spawnQuery) {
        std::string trimmedQuery = Trim(spawnQuery);
        if (trimmedQuery.empty()) {
            SetStatus("Enter a vehicle name first.");
            return false;
        }

        auto* playerController = SDK::GetPlayerController();
        if (!playerController) {
            SetStatus("Player controller not available.");
            return false;
        }

        if (!ContainsDigit(trimmedQuery)) {
            trimmedQuery += " 1";
        }

        if (!playerController->CheatManager) {
            playerController->EnableCheats();
        }

        auto* cheatManager = playerController->CheatManager;
        if (cheatManager && cheatManager->IsA(SDK::USN2CheatManager::StaticClass())) {
            auto* sn2CheatManager = static_cast<SDK::USN2CheatManager*>(cheatManager);
            const std::wstring wideQuery = Utils::Helpers::StringToWideString(trimmedQuery.c_str());
            sn2CheatManager->Spawn(SDK::FString(wideQuery.c_str()));
            SetStatus("Issued vehicle spawn: " + trimmedQuery + ".");
            return true;
        }

        auto* world = SDK::UWorld::GetWorld();
        if (!world) {
            SetStatus("World not available for spawn fallback.");
            return false;
        }

        const std::wstring wideCommand = Utils::Helpers::StringToWideString(("spawn " + trimmedQuery).c_str());
        SDK::USN2Statics::ConsoleCommand(world, SDK::FString(wideCommand.c_str()));
        SetStatus("Issued vehicle spawn through console fallback: " + trimmedQuery + ".");
        return true;
    }

    const std::string& GetLastStatus() {
        return g_lastStatus;
    }
}
