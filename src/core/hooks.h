#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <functional>

namespace Hooks {
    void Initialize();
    void Shutdown();
    
    // Hook types
    using HookCallback = std::function<void()>;
    
    // D3D11 Hook
    namespace D3D11 {
        void Hook();
        void Unhook();
        
        // Present hook callback
        using PresentCallbackType = HRESULT(__stdcall*)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
        extern PresentCallbackType PresentCallback;
    }
    
    // Key input hook
    namespace Input {
        void ProcessKeyInput(int key, bool isDown);
    }
}
