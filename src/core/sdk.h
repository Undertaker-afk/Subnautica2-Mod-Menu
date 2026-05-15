#pragma once

#include <Windows.h>
#include <cstdint>

// Core SDK interface
namespace SDK {
    void Initialize();
    void Shutdown();
    
    // Get game objects
    class ASN2PlayerCharacter* GetLocalPlayer();
    class ABP_SN2PlayerController* GetPlayerController();
    
    // GObjects
    extern uintptr_t GObjects;
    extern uintptr_t GNames;
    extern uintptr_t GWorld;
    
    // Offsets
    namespace Offsets {
        inline constexpr int32_t GObjects = 215045376;  // Will be filled from dump
        inline constexpr int32_t GNames = 214110976;
        inline constexpr int32_t GWorld = 211655200;
    }
}

// Feature toggles
namespace Features {
    inline bool bGodMode = false;
    inline bool InfiniteOxygen = false;
    inline bool InfiniteBattery = false;
    inline bool SpeedHack = false;
    inline float SpeedMultiplier = 2.0f;
    inline bool bNoClip = false;
    inline bool MaterialESP = false;
    inline bool PlayerESP = false;
    inline bool CreatureESP = false;
    inline bool ItemESP = false;
}

// Menu state
namespace Menu {
    inline bool IsOpen = false;
    inline const char* Title = "Subnautica 2 Mod Menu";
}
