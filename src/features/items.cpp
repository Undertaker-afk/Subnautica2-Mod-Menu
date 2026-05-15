#include "items.h"
#include "../core/sdk.h"
#include "../utils/helpers.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"
#include <algorithm>
#include <cctype>

namespace Features::Items {
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

        void SetStatus(std::string status) {
            g_lastStatus = std::move(status);
            Utils::Helpers::Log("[ModMenu][Items] %s", g_lastStatus.c_str());
        }
    }

    bool GiveItem(const std::string& itemName, int quantity) {
        const std::string trimmedName = Trim(itemName);
        if (trimmedName.empty()) {
            SetStatus("Enter an item name first.");
            return false;
        }

        if (quantity <= 0) {
            SetStatus("Quantity must be at least 1.");
            return false;
        }

        auto* world = SDK::UWorld::GetWorld();
        if (!world) {
            SetStatus("World not available.");
            return false;
        }

        auto* inventory = SDK::USN2Statics::GetLocalPlayerInventory(world);
        if (!inventory) {
            SetStatus("Player inventory not available.");
            return false;
        }

        bool found = false;
        const std::wstring wideName = Utils::Helpers::StringToWideString(trimmedName.c_str());
        auto* itemType = SDK::UUWEInventoryStatics::GetItemTypeFromName(SDK::FString(wideName.c_str()), true, &found);
        if (!found || !itemType) {
            SetStatus("Item not found in the current item database.");
            return false;
        }

        const int added = inventory->AddAsMuchAsPossibleToInventory(itemType, quantity);
        if (added <= 0) {
            SetStatus("Could not add the item. Inventory may be full.");
            return false;
        }

        if (added < quantity) {
            SetStatus("Added " + std::to_string(added) + "/" + std::to_string(quantity) + " item(s). Inventory filled up.");
            return true;
        }

        SetStatus("Added " + std::to_string(added) + " item(s) of " + trimmedName + ".");
        return true;
    }

    const std::string& GetLastStatus() {
        return g_lastStatus;
    }
}
