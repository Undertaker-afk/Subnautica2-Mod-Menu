#include "battery.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Battery {
    namespace {
        constexpr float kEnergyEpsilon = 0.01f;

        void RefillEnergy(SDK::ASN2PlayerCharacter* player) {
            if (!player || !player->MechanicalSetComponent) {
                return;
            }

            auto* mechanical = player->MechanicalSetComponent;
            const float maxEnergy = mechanical->GetMaxEnergy();
            if (maxEnergy <= 0.0f) {
                return;
            }

            if ((maxEnergy - mechanical->GetEnergy()) <= kEnergyEpsilon) {
                return;
            }

            mechanical->SetEnergy(maxEnergy);
        }
    }

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->MechanicalSetComponent) return;

        RefillEnergy(player);
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

        RefillEnergy(player);
    }
}
