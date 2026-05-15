#include "esp.h"
#include "../core/sdk.h"
#include "../ui/renderer.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <Windows.h>

namespace Features::ESP {
    struct ESPObject {
        SDK::AActor* actor;
        std::string name;
        SDK::FVector location;
        bool isPlayer;
        bool isCreature;
        bool isItem;
    };

    static std::string FStringToUtf8(const SDK::FString& value) {
        const auto* wide = value.c_str();
        if (!wide) {
            return {};
        }

        int size = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
        if (size <= 0) {
            return {};
        }

        std::string utf8;
        utf8.resize(static_cast<size_t>(size) - 1);
        WideCharToMultiByte(CP_UTF8, 0, wide, -1, utf8.data(), size, nullptr, nullptr);
        return utf8;
    }

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
            obj.isCreature = false;
            obj.isItem = false;
            obj.name.clear();

            // Get class name
            auto* cls = actor->GetClass();
            if (cls) {
                auto className = FStringToUtf8(cls->GetName());
                obj.name = className;

                std::string lowerName = className;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
                obj.isCreature = lowerName.find("creature") != std::string::npos;
                obj.isItem = actor->IsA(SDK::ASN2PickupItem::StaticClass()) || lowerName.find("pickup") != std::string::npos;
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
            const float dx = playerLoc.X - obj.location.X;
            const float dy = playerLoc.Y - obj.location.Y;
            const float dz = playerLoc.Z - obj.location.Z;
            const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            const std::string distText = std::to_string(static_cast<int>(dist));

            // World to screen
            SDK::FVector2D screenPos;
            auto* controller = SDK::GetPlayerController();
            if (controller && controller->ProjectWorldLocationToScreen(obj.location, &screenPos, true)) {
                // Draw based on type
                if (obj.isPlayer && Features::PlayerESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y, 
                        "Player", ImColor(0, 255, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
                else if (obj.isCreature && Features::CreatureESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y,
                        obj.name.c_str(), ImColor(255, 0, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
                else if (obj.isItem && Features::ItemESP) {
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y,
                        obj.name.c_str(), ImColor(255, 255, 0, 255), 12.0f);
                    UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14,
                        distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
                }
            }
        }
    }
}
