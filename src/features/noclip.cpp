#include "noclip.h"
#include "../core/sdk.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"

namespace Features::NoClip {
    static bool bOriginalCollisionEnabled = true;

    void Enable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player) return;

        // Store original collision state
        auto* mesh = player->Mesh;
        if (mesh) {
            bOriginalCollisionEnabled = mesh->IsCollisionEnabled();
            mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        // Disable gravity
        auto* movement = player->CharacterMovement;
        if (movement) {
            movement->GravityScale = 0.0f;
        }
    }

    void Disable() {
        auto* player = SDK::GetLocalPlayer();
        if (!player) return;

        // Restore collision
        auto* mesh = player->Mesh;
        if (mesh) {
            mesh->SetCollisionEnabled(bOriginalCollisionEnabled ? 
                ECollisionEnabled::QueryAndPhysics : 
                ECollisionEnabled::NoCollision);
        }

        // Restore gravity
        auto* movement = player->CharacterMovement;
        if (movement) {
            movement->GravityScale = 1.0f;
        }
    }

    void Update() {
        if (!Features::bNoClip) {
            Disable();
            return;
        }

        auto* player = SDK::GetLocalPlayer();
        if (!player) return;

        // Keep collision disabled and gravity off
        auto* mesh = player->Mesh;
        if (mesh) {
            mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }

        auto* movement = player->CharacterMovement;
        if (movement) {
            movement->GravityScale = 0.0f;
        }
    }
}
