#include "oxygen.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::Oxygen {
    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player || !player->SurvivalSetComponent) return;
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
    }
}
