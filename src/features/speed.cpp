#include "speed.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Speed {
    static float originalWalkSpeed = 0.0f;
    static float originalSwimSpeed = 0.0f;
    static SDK::ASN2PlayerCharacter* cachedPlayer = nullptr;
    static bool hasOriginalSpeeds = false;

    namespace {
        void CaptureOriginalSpeeds(SDK::ASN2PlayerCharacter* player) {
            if (!player || !player->MovementSetComponent) {
                return;
            }

            if (cachedPlayer != player || !hasOriginalSpeeds) {
                originalWalkSpeed = player->MovementSetComponent->GetBaseWalkSpeed();
                originalSwimSpeed = player->MovementSetComponent->GetBaseSwimSpeed();
                cachedPlayer = player;
                hasOriginalSpeeds = true;
            }
        }

        void ApplySpeedMultiplier(SDK::ASN2PlayerCharacter* player) {
            if (!player || !player->MovementSetComponent) {
                return;
            }

            CaptureOriginalSpeeds(player);
            player->MovementSetComponent->SetBaseWalkSpeed(originalWalkSpeed * Features::SpeedMultiplier);
            player->MovementSetComponent->SetBaseSwimSpeed(originalSwimSpeed * Features::SpeedMultiplier);
            player->MovementSetComponent->SetRotationSpeedMultiplier(Features::SpeedMultiplier);
        }
    }

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MovementSetComponent) return;

        ApplySpeedMultiplier(player);
    }

    void Disable() {
        auto* player = SDK::GetLocalPlayer();
        if (player && player->MovementSetComponent && hasOriginalSpeeds) {
            player->MovementSetComponent->SetBaseWalkSpeed(originalWalkSpeed);
            player->MovementSetComponent->SetBaseSwimSpeed(originalSwimSpeed);
            player->MovementSetComponent->SetRotationSpeedMultiplier(1.0f);
        }
    }

    void Update() {
        if (!Features::SpeedHack) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MovementSetComponent) return;

        ApplySpeedMultiplier(player);
    }
}
