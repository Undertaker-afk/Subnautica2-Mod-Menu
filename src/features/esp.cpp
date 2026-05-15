#include "esp.h"
#include "../core/sdk.h"
#include "../ui/renderer.h"
#include "../../../5.6.1-113109+++Project+SN2-Release-Hotfix-Live-Subnautica2/CppSDK/SDK.hpp"
#include <algorithm>
#include <cmath>
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

namespace Features::ESP {
    namespace {
        struct ESPObject {
            SDK::AActor* actor;
            std::string name;
            SDK::FVector location;
            bool isPlayer;
            bool isCreature;
            bool isItem;
            bool isSelectedResource;
        };

        static std::vector<ResourceFilterEntry> resourceFilters;
        static std::unordered_map<std::string, bool> resourceSelections;
        static std::vector<ESPObject> objects;

        std::string ToLowerCopy(const std::string& value) {
            std::string lowered = value;
            std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });
            return lowered;
        }

        bool LooksLikePlayer(const std::string& lowerName, SDK::AActor* actor) {
            return actor->IsA(SDK::ASN2PlayerCharacter::StaticClass()) ||
                lowerName.find("player") != std::string::npos;
        }

        bool LooksLikeCreature(const std::string& lowerName) {
            return lowerName.find("creature") != std::string::npos ||
                lowerName.find("fauna") != std::string::npos;
        }

        bool LooksLikeItem(const std::string& lowerName, SDK::AActor* actor) {
            return actor->IsA(SDK::ASN2PickupItem::StaticClass()) ||
                lowerName.find("pickup") != std::string::npos ||
                lowerName.find("item") != std::string::npos;
        }

        std::vector<std::pair<std::string, bool>> CollectDiscoveredResourceNames() {
            std::vector<std::pair<std::string, bool>> discovered;

            auto* world = SDK::UWorld::GetWorld();
            if (!world || !world->PersistentLevel) {
                return discovered;
            }

            for (auto* actor : world->PersistentLevel->Actors) {
                if (!actor || !actor->Class) {
                    continue;
                }

                const std::string className = actor->Class->GetName();
                if (className.empty()) {
                    continue;
                }

                const std::string lowerName = ToLowerCopy(className);
                const bool isPlayer = LooksLikePlayer(lowerName, actor);
                const bool isCreature = LooksLikeCreature(lowerName);
                const bool isItem = LooksLikeItem(lowerName, actor);

                if (isPlayer || isCreature || isItem) {
                    continue;
                }

                discovered.emplace_back(className, resourceSelections[className]);
            }

            std::sort(discovered.begin(), discovered.end(), [](const auto& left, const auto& right) {
                return left.first < right.first;
            });

            discovered.erase(std::unique(discovered.begin(), discovered.end(), [](const auto& left, const auto& right) {
                return left.first == right.first;
            }), discovered.end());

            return discovered;
        }

        void CollectObjects() {
            objects.clear();

            auto* world = SDK::UWorld::GetWorld();
            if (!world || !world->PersistentLevel) {
                return;
            }

            const bool hasSelectedResources = HasAnySelectedResourceFilter();

            for (auto* actor : world->PersistentLevel->Actors) {
                if (!actor) {
                    continue;
                }

                ESPObject obj{};
                obj.actor = actor;
                obj.location = actor->K2_GetActorLocation();

                if (actor->Class) {
                    obj.name = actor->Class->GetName();
                }

                const std::string lowerName = ToLowerCopy(obj.name);
                obj.isPlayer = LooksLikePlayer(lowerName, actor);
                obj.isCreature = LooksLikeCreature(lowerName);
                obj.isItem = LooksLikeItem(lowerName, actor);
                obj.isSelectedResource = Features::MaterialESP && hasSelectedResources &&
                    !obj.isPlayer && !obj.isCreature && !obj.isItem &&
                    resourceSelections.contains(obj.name) && resourceSelections[obj.name];

                if ((Features::PlayerESP && obj.isPlayer) ||
                    (Features::CreatureESP && obj.isCreature) ||
                    (Features::ItemESP && obj.isItem) ||
                    obj.isSelectedResource) {
                    objects.push_back(obj);
                }
            }
        }
    }

    void Initialize() {
        objects.clear();
        resourceFilters.clear();
        resourceSelections.clear();
    }

    void Shutdown() {
        objects.clear();
        resourceFilters.clear();
        resourceSelections.clear();
    }

    void RefreshResourceFilters() {
        resourceFilters.clear();

        for (const auto& [name, selected] : CollectDiscoveredResourceNames()) {
            resourceFilters.push_back({name, selected});
        }
    }

    const std::vector<ResourceFilterEntry>& GetResourceFilters() {
        return resourceFilters;
    }

    void SetResourceFilterSelected(std::size_t index, bool selected) {
        if (index >= resourceFilters.size()) {
            return;
        }

        resourceFilters[index].selected = selected;
        resourceSelections[resourceFilters[index].label] = selected;
    }

    void SetAllResourceFilters(bool selected) {
        for (auto& entry : resourceFilters) {
            entry.selected = selected;
            resourceSelections[entry.label] = selected;
        }
    }

    bool HasAnySelectedResourceFilter() {
        return std::any_of(resourceFilters.begin(), resourceFilters.end(), [](const ResourceFilterEntry& entry) {
            return entry.selected;
        });
    }

    void Render() {
        if (!Features::MaterialESP && !Features::PlayerESP &&
            !Features::CreatureESP && !Features::ItemESP) {
            return;
        }

        RefreshResourceFilters();
        CollectObjects();

        auto* player = SDK::GetLocalPlayer();
        if (!player) {
            return;
        }

        const auto playerLoc = player->K2_GetActorLocation();

        for (const auto& obj : objects) {
            const float dx = static_cast<float>(playerLoc.X - obj.location.X);
            const float dy = static_cast<float>(playerLoc.Y - obj.location.Y);
            const float dz = static_cast<float>(playerLoc.Z - obj.location.Z);
            const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            const std::string distText = std::to_string(static_cast<int>(dist)) + "m";

            SDK::FVector2D screenPos;
            auto* controller = SDK::GetPlayerController();
            if (!controller || !controller->ProjectWorldLocationToScreen(obj.location, &screenPos, true)) {
                continue;
            }

            if (obj.isPlayer && Features::PlayerESP) {
                UI::Renderer::DrawText(screenPos.X, screenPos.Y, "Player", ImColor(0, 255, 0, 255), 12.0f);
                UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14.0f, distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
            }
            else if (obj.isCreature && Features::CreatureESP) {
                UI::Renderer::DrawText(screenPos.X, screenPos.Y, obj.name.c_str(), ImColor(255, 80, 80, 255), 12.0f);
                UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14.0f, distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
            }
            else if (obj.isItem && Features::ItemESP) {
                UI::Renderer::DrawText(screenPos.X, screenPos.Y, obj.name.c_str(), ImColor(255, 255, 0, 255), 12.0f);
                UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14.0f, distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
            }
            else if (obj.isSelectedResource) {
                UI::Renderer::DrawText(screenPos.X, screenPos.Y, obj.name.c_str(), ImColor(0, 220, 255, 255), 12.0f);
                UI::Renderer::DrawText(screenPos.X, screenPos.Y + 14.0f, distText.c_str(), ImColor(255, 255, 255, 255), 10.0f);
            }
        }
    }
}
