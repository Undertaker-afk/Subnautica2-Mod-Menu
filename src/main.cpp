#include <Windows.h>
#include "core/sdk.h"
#include "core/hooks.h"
#include "core/config.h"
#include "features/godmode.h"
#include "features/oxygen.h"
#include "features/battery.h"
#include "features/speed.h"
#include "features/noclip.h"
#include "features/esp.h"
#include "utils/helpers.h"

// Global thread for feature updates
static HANDLE g_updateThread = nullptr;
static bool g_running = false;

// Update thread function
DWORD WINAPI UpdateThread(LPVOID lpParam) {
    Utils::Helpers::Log("[ModMenu] Update thread started");
    
    while (g_running) {
        // Update all enabled features
        if (Features::bGodMode) {
            Features::GodMode::Update();
        }
        
        if (Features::InfiniteOxygen) {
            Features::Oxygen::Update();
        }
        
        if (Features::InfiniteBattery) {
            Features::Battery::Update();
        }
        
        if (Features::SpeedHack) {
            Features::Speed::Update();
        }
        
        if (Features::bNoClip) {
            Features::NoClip::Update();
        }
        
        // Render ESP
        Features::ESP::Render();
        
        Sleep(16); // ~60 FPS
    }
    
    Utils::Helpers::Log("[ModMenu] Update thread stopped");
    return 0;
}

// Main initialization
void InitializeModMenu() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    
    Utils::Helpers::Log("[ModMenu] =========================================");
    Utils::Helpers::Log("[ModMenu] Subnautica 2 Mod Menu v1.0");
    Utils::Helpers::Log("[ModMenu] =========================================");
    
    // Initialize SDK
    SDK::Initialize();
    Utils::Helpers::Log("[ModMenu] SDK initialized");
    
    // Initialize ESP
    Features::ESP::Initialize();
    Utils::Helpers::Log("[ModMenu] ESP initialized");
    
    // Load config
    Config::Load("modmenu_config.ini");
    Utils::Helpers::Log("[ModMenu] Config loaded");
    
    // Initialize hooks (D3D11 present hook for ImGui)
    Hooks::Initialize();
    Utils::Helpers::Log("[ModMenu] Hooks initialized");
    
    // Start update thread
    g_running = true;
    g_updateThread = CreateThread(nullptr, 0, UpdateThread, nullptr, 0, nullptr);
    Utils::Helpers::Log("[ModMenu] Update thread started");
    
    Utils::Helpers::Log("[ModMenu] Mod Menu fully initialized!");
    Utils::Helpers::Log("[ModMenu] Press INSERT to toggle menu");
}

// Cleanup function
void CleanupModMenu() {
    Utils::Helpers::Log("[ModMenu] Cleaning up...");
    
    // Stop update thread
    g_running = false;
    if (g_updateThread) {
        WaitForSingleObject(g_updateThread, 1000);
        CloseHandle(g_updateThread);
    }
    
    // Shutdown hooks
    Hooks::Shutdown();
    
    // Shutdown ESP
    Features::ESP::Shutdown();
    
    // Shutdown SDK
    SDK::Shutdown();
    
    // Save config
    Config::Save("modmenu_config.ini");
    
    Utils::Helpers::Log("[ModMenu] Cleanup complete");
    
    // Free console
    FreeConsole();
}

// DLL Entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitializeModMenu, nullptr, 0, nullptr);
        break;
        
    case DLL_PROCESS_DETACH:
        CleanupModMenu();
        break;
    }
    return TRUE;
}
