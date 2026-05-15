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

namespace UI::Menu {
    void Render() {
        if (!Menu::IsOpen) return;

        // Main window
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin(Menu::Title, &Menu::IsOpen, ImGuiWindowFlags_NoCollapse);

        // Tab bar
        if (ImGui::BeginTabBar("ModMenuTabs")) {
            // Player Tab
            if (ImGui::BeginTabItem("Player")) {
                ImGui::Checkbox("God Mode", &Features::bGodMode);
                if (Features::bGodMode) {
                    Features::GodMode::Enable();
                }

                ImGui::Checkbox("Infinite Oxygen", &Features::InfiniteOxygen);
                if (Features::InfiniteOxygen) {
                    Features::Oxygen::Enable();
                }

                ImGui::Checkbox("Speed Hack", &Features::SpeedHack);
                if (Features::SpeedHack) {
                    ImGui::SliderFloat("Speed Multiplier", &Features::SpeedMultiplier, 1.0f, 10.0f, "%.1f");
                    Features::Speed::Enable();
                }

                ImGui::Checkbox("No Clip", &Features::bNoClip);
                if (Features::bNoClip) {
                    Features::NoClip::Enable();
                }

                ImGui::EndTabItem();
            }

            // Tools Tab
            if (ImGui::BeginTabItem("Tools")) {
                ImGui::Checkbox("Infinite Battery", &Features::InfiniteBattery);
                if (Features::InfiniteBattery) {
                    Features::Battery::Enable();
                }

                // Add more tool cheats here
                ImGui::Text("More tool cheats coming soon...");

                ImGui::EndTabItem();
            }

            // ESP Tab
            if (ImGui::BeginTabItem("ESP")) {
                ImGui::Checkbox("Material ESP", &Features::MaterialESP);
                ImGui::Checkbox("Player ESP", &Features::PlayerESP);
                ImGui::Checkbox("Creature ESP", &Features::CreatureESP);
                ImGui::Checkbox("Item ESP", &Features::ItemESP);

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
