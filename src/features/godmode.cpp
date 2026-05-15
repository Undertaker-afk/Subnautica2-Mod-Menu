#include "godmode.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::GodMode {
    static float originalMaxHealth = 100.0f;

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->HealthSetComponent) return;

        // Store original max health
        originalMaxHealth = player->HealthSetComponent->GetMaxHealth();

        // Refill health using the current SDK setter.
        player->HealthSetComponent->SetHealth(originalMaxHealth);
    }

    void Disable() {
        // Restore original health state
        auto* player = SDK::GetLocalPlayer();
        if (player && player->HealthSetComponent) {
            player->HealthSetComponent->SetHealth(originalMaxHealth);
        }
    }

    void Update() {
        if (!Features::bGodMode) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->HealthSetComponent) return;

        // Keep health at max
        float maxHealth = player->HealthSetComponent->GetMaxHealth();
        player->HealthSetComponent->SetHealth(maxHealth);
    }
}
