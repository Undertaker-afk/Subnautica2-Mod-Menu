#include "battery.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Battery {
    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MechanicalSetComponent) return;

        // Set energy to max
        float maxEnergy = player->MechanicalSetComponent->GetMaxEnergy();
        player->MechanicalSetComponent->SetEnergy(maxEnergy);
    }

    void Disable() {
        // Let battery deplete normally
    }

    void Update() {
        if (!Features::InfiniteBattery) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MechanicalSetComponent) return;

        // Keep energy at max
        float maxEnergy = player->MechanicalSetComponent->GetMaxEnergy();
        player->MechanicalSetComponent->SetEnergy(maxEnergy);
    }
}
