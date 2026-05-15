#include "speed.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Speed {
    static float originalWalkSpeed = 0.0f;
    static float originalSwimSpeed = 0.0f;

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MovementSetComponent) return;

        // Store original speeds
        originalWalkSpeed = player->MovementSetComponent->GetBaseWalkSpeed();
        originalSwimSpeed = player->MovementSetComponent->GetBaseSwimSpeed();

        // Apply speed multiplier
        player->MovementSetComponent->SetBaseWalkSpeed(originalWalkSpeed * Features::SpeedMultiplier);
        player->MovementSetComponent->SetBaseSwimSpeed(originalSwimSpeed * Features::SpeedMultiplier);
    }

    void Disable() {
        auto* player = SDK::GetLocalPlayer();
        if (player && player->MovementSetComponent) {
            // Restore original speeds
            player->MovementSetComponent->SetBaseWalkSpeed(originalWalkSpeed);
            player->MovementSetComponent->SetBaseSwimSpeed(originalSwimSpeed);
        }
    }

    void Update() {
        if (!Features::SpeedHack) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MovementSetComponent) return;

        // Apply speed multiplier
        player->MovementSetComponent->SetBaseWalkSpeed(originalWalkSpeed * Features::SpeedMultiplier);
        player->MovementSetComponent->SetBaseSwimSpeed(originalSwimSpeed * Features::SpeedMultiplier);
    }
}
