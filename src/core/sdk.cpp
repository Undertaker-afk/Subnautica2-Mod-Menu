#include "sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace SDK {
    uintptr_t GObjects = 0;
    uintptr_t GNames = 0;
    uintptr_t GWorld = 0;

    void Initialize() {
        // SDK is already generated, we can use it directly
        // Initialize any required hooks or patches here
    }

    void Shutdown() {
        // Cleanup hooks and patches
    }

    SDK::ASN2PlayerCharacter* GetLocalPlayer() {
        auto* world = SDK::UWorld::GetWorld();
        if (!world) return nullptr;

        auto* gameInstance = world->OwningGameInstance;
        if (!gameInstance) return nullptr;

        for (auto* localPlayer : gameInstance->LocalPlayers) {
            if (!localPlayer) continue;

            auto* playerController = localPlayer->PlayerController;
            if (!playerController) continue;

            auto* pawn = playerController->AcknowledgedPawn;
            if (!pawn) continue;

            // Check if it's the player character
            if (pawn->IsA(SDK::ASN2PlayerCharacter::StaticClass())) {
                return static_cast<SDK::ASN2PlayerCharacter*>(pawn);
            }
        }

        return nullptr;
    }

    SDK::ABP_SN2PlayerController* GetPlayerController() {
        auto* world = SDK::UWorld::GetWorld();
        if (!world) return nullptr;

        auto* gameInstance = world->OwningGameInstance;
        if (!gameInstance) return nullptr;

        for (auto* localPlayer : gameInstance->LocalPlayers) {
            if (!localPlayer) continue;

            auto* playerController = localPlayer->PlayerController;
            if (!playerController) continue;

            if (playerController->IsA(SDK::ABP_SN2PlayerController::StaticClass())) {
                return static_cast<SDK::ABP_SN2PlayerController*>(playerController);
            }
        }

        return nullptr;
    }
}
