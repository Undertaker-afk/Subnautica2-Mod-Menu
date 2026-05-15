#include "oxygen.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Oxygen {
    namespace {
        constexpr float kOxygenEpsilon = 0.01f;

        SDK::UUWESurvivalAttributeSet* GetSurvivalAttributes(SDK::ASN2PlayerCharacter* player) {
            if (!player || !player->SurvivalSetComponent) {
                return nullptr;
            }

            auto* asc = player->SurvivalSetComponent->AbilitySystemComponent;
            if (!asc) {
                return nullptr;
            }

            auto* attributes = asc->GetAttributeSet(SDK::UUWESurvivalAttributeSet::StaticClass());
            return const_cast<SDK::UUWESurvivalAttributeSet*>(static_cast<const SDK::UUWESurvivalAttributeSet*>(attributes));
        }

        void RefillOxygen(SDK::ASN2PlayerCharacter* player) {
            auto* attributes = GetSurvivalAttributes(player);
            if (!attributes || !player->SurvivalSetComponent) {
                return;
            }

            const float maxOxygen = player->SurvivalSetComponent->GetMaxOxygen();
            if (maxOxygen <= 0.0f) {
                return;
            }

            if ((maxOxygen - attributes->Oxygen.CurrentValue) <= kOxygenEpsilon) {
                return;
            }

            attributes->Oxygen.BaseValue = maxOxygen;
            attributes->Oxygen.CurrentValue = maxOxygen;
        }
    }

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->SurvivalSetComponent) return;

        RefillOxygen(player);
    }

    void Disable() {
        // Let oxygen deplete normally
    }

    void Update() {
        if (!Features::InfiniteOxygen) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->SurvivalSetComponent) return;

        RefillOxygen(player);
    }
}
