#include "menu.h"
#include "renderer.h"
#include "../core/sdk.h"
#include "../features/godmode.h"
#include "../features/oxygen.h"
#include "../features/battery.h"
#include "../features/speed.h"
#include "../features/noclip.h"
#include "../features/esp.h"
#include <imgui.h>
#include <algorithm>
#include <cctype>
#include <string>

namespace UI::Menu {
    void Render() {
        if (!::Menu::IsOpen) return;

        // Main window
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin(::Menu::Title, &::Menu::IsOpen, ImGuiWindowFlags_NoCollapse);

        // Tab bar
        if (ImGui::BeginTabBar("ModMenuTabs")) {
            // Player Tab
            if (ImGui::BeginTabItem("Player")) {
                ImGui::Checkbox("God Mode", &Features::bGodMode);
                if (Features::bGodMode) {
                    Features::GodMode::Enable();
                }

                if (ImGui::Checkbox("Infinite Oxygen", &Features::InfiniteOxygen)) {
                    if (Features::InfiniteOxygen) {
                        Features::Oxygen::Enable();
                    } else {
                        Features::Oxygen::Disable();
                    }
                }

                if (ImGui::Checkbox("Speed Hack", &Features::SpeedHack)) {
                    if (Features::SpeedHack) {
                        Features::Speed::Enable();
                    } else {
                        Features::Speed::Disable();
                    }
                }
                if (Features::SpeedHack) {
                    ImGui::SliderFloat("Speed Multiplier", &Features::SpeedMultiplier, 1.0f, 10.0f, "%.1f");
                }

                ImGui::Checkbox("No Clip", &Features::bNoClip);
                if (Features::bNoClip) {
                    Features::NoClip::Enable();
                }

                ImGui::EndTabItem();
            }

            // Tools Tab
            if (ImGui::BeginTabItem("Tools")) {
                if (ImGui::Checkbox("Infinite Battery", &Features::InfiniteBattery)) {
                    if (Features::InfiniteBattery) {
                        Features::Battery::Enable();
                    } else {
                        Features::Battery::Disable();
                    }
                }

                // Add more tool cheats here
                ImGui::Text("More tool cheats coming soon...");

                ImGui::EndTabItem();
            }

            // ESP Tab
            if (ImGui::BeginTabItem("ESP")) {
                ImGui::Checkbox("Resource ESP", &Features::MaterialESP);
                ImGui::Checkbox("Player ESP", &Features::PlayerESP);
                ImGui::Checkbox("Creature ESP", &Features::CreatureESP);
                ImGui::Checkbox("Item ESP", &Features::ItemESP);

                if (Features::MaterialESP) {
                    static char resourceSearch[128] = "";
                    Features::ESP::RefreshResourceFilters();

                    ImGui::Separator();
                    ImGui::Text("Discovered Classes");
                    ImGui::InputTextWithHint("##resource-search", "Search dumped classes...", resourceSearch, IM_ARRAYSIZE(resourceSearch));

                    if (ImGui::Button("Select All")) {
                        Features::ESP::SetAllResourceFilters(true);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Clear")) {
                        Features::ESP::SetAllResourceFilters(false);
                    }

                    std::string searchText = resourceSearch;
                    std::transform(searchText.begin(), searchText.end(), searchText.begin(), [](unsigned char c) {
                        return static_cast<char>(std::tolower(c));
                    });

                    ImGui::BeginChild("resource-filter-list", ImVec2(0.0f, 220.0f), true);

                    const auto& resourceFilters = Features::ESP::GetResourceFilters();
                    for (std::size_t i = 0; i < resourceFilters.size(); ++i) {
                        const auto& entry = resourceFilters[i];
                        std::string lowerLabel = entry.label;
                        std::transform(lowerLabel.begin(), lowerLabel.end(), lowerLabel.begin(), [](unsigned char c) {
                            return static_cast<char>(std::tolower(c));
                        });

                        if (!searchText.empty() && lowerLabel.find(searchText) == std::string::npos) {
                            continue;
                        }

                        bool selected = entry.selected;
                        if (ImGui::Checkbox(entry.label.c_str(), &selected)) {
                            Features::ESP::SetResourceFilterSelected(i, selected);
                        }
                    }

                    ImGui::EndChild();

                    if (!Features::ESP::HasAnySelectedResourceFilter()) {
                        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.3f, 1.0f), "Select dumped classes to draw them in ESP.");
                    }
                }

                ImGui::EndTabItem();
            }

            // Teleport Tab
            if (ImGui::BeginTabItem("Teleport")) {
                ImGui::Text("Teleport cheats coming soon...");
                ImGui::EndTabItem();
            }

            // Settings Tab
            if (ImGui::BeginTabItem("Settings")) {
                ImGui::Text("Menu Settings");
                ImGui::Separator();
                
                static bool showFPS = false;
                ImGui::Checkbox("Show FPS", &showFPS);
                
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        // Footer
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "INSERT to toggle menu | Subnautica 2 Mod Menu v1.0");

        ImGui::End();
    }
}
