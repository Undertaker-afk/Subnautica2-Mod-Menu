#include "esp.h"
#include "../core/sdk.h"
#include "../ui/renderer.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"
#include <vector>
#include <string>

namespace Features::ESP {
    struct ESPObject {
        SDK::AActor* actor;
        std::string name;
        SDK::FVector location;
        bool isPlayer;
        bool isCreature;
        bool isItem;
    };

    static std::vector<ESPObject> objects;

    void Initialize() {
        objects.clear();
    }

    void Shutdown() {
        objects.clear();
    }

    static void CollectObjects() {
        objects.clear();

        auto* world = SDK::UWorld::GetWorld();
        if (!world) return;

        auto* level = world->PersistentLevel;
        if (!level) return;

        for (auto* actor : level->Actors) {
            if (!actor) continue;

            ESPObject obj;
            obj.actor = actor;
            obj.location = actor->K2_GetActorLocation();
            obj.isPlayer = actor->IsA(SDK::ASN2PlayerCharacter::StaticClass());
            obj.isCreature = actor->IsA(SDK::ASN2BaseCreature::StaticClass());
            obj.isItem = actor->IsA(SDK::AUWEItemPickup::StaticClass());

            // Get class name
            auto* cls = actor->GetClass();
            if (cls) {
                auto name = cls->GetName();
                obj.name = std::string(name.begin(), name.end());
            }

            // Filter based on enabled features
            if ((Features::PlayerESP && obj.isPlayer) ||
                (Features::CreatureESP && obj.isCreature) ||
                (Features::ItemESP && obj.isItem) ||
                Features::MaterialESP) {
                objects.push_back(obj);
            }
        }
    }

    void Render() {
        if (!Features::MaterialESP && !Features::PlayerESP && 
            !Features::CreatureESP && !Features::ItemESP) {
            return;
        }

        CollectObjects();

        auto* player = SDK::GetLocalPlayer();
        if (!player) return;

        auto playerLoc = player->K2_GetActorLocation();

        for (const auto& obj : objects) {
            // Calculate distance
            float dist = SDK::FVector::Distance(playerLoc, obj.location);

            // World to screen
            SDK::FVector2D screenPos;
            auto* controller = SDK::GetPlayerController();
            if (controller && controller->ProjectWorldLocationToScreen(obj.location, &screenPos, true)) {
                // Draw based on type
                if (obj.isPlayer && Features::PlayerESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y, 
                        "Player", ImColor(0, 255, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        std::to_string((int)dist).c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
                else if (obj.isCreature && Features::CreatureESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y,
                        obj.name.c_str(), ImColor(255, 0, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        std::to_string((int)dist).c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
                else if (obj.isItem && Features::ItemESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y,
                        obj.name.c_str(), ImColor(255, 255, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        std::to_string((int)dist).c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
            }
        }
    }
}
