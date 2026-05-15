#include "hooks.h"
#include "sdk.h"
#include "../ui/menu.h"
#include "../ui/renderer.h"
#include <d3d11.h>
#include <dxgi.h>

namespace Hooks {
    // Original function pointers
    static HRESULT(__stdcall* oPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) = nullptr;
    
    // D3D11 state
    static ID3D11Device* g_pd3dDevice = nullptr;
    static ID3D11DeviceContext* g_pd3dContext = nullptr;
    static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    static IDXGISwapChain* g_pSwapChain = nullptr;
    
    void (*D3D11::PresentCallback)(IDXGISwapChain*, UINT, UINT) = nullptr;

    // Hooked Present function
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        // Initialize D3D11 on first call
        static bool initialized = false;
        if (!initialized) {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {
                g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
                
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                
                // Create renderer
                UI::Renderer::Initialize(g_pd3dDevice, g_pd3dContext, sd.OutputWindow);
                
                initialized = true;
            }
        }
        
        // Render menu if open
        if (initialized && UI::Renderer::IsReady()) {
            // Create or get render target
            if (!g_mainRenderTargetView) {
                ID3D11Texture2D* pBackBuffer = nullptr;
                pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
                g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
                pBackBuffer->Release();
            }
            
            // Render menu
            UI::Renderer::NewFrame();
            UI::Menu::Render();
            UI::Renderer::EndFrame(g_mainRenderTargetView);
        }
        
        // Call original
        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    void D3D11::Hook() {
        // Find D3D11 device and swap chain
        WNDCLASSEX wc = { sizeof(wc) };
        wc.lpfnWndProc = DefWindowProc;
        wc.lpszClassName = TEXT("D3D11Hook");
        RegisterClassEx(&wc);
        
        HWND hwnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
        
        D3D_FEATURE_LEVEL featureLevel;
        DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = 100;
        swapChainDesc.BufferDesc.Height = 100;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        IDXGISwapChain* swapChain = nullptr;
        
        if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &swapChain,
            &device,
            &featureLevel,
            &context))) {
            
            // Get vtable
            void** pVTable = *reinterpret_cast<void***>(swapChain);
            
            // Hook Present (index 8)
            oPresent = reinterpret_cast<decltype(oPresent)>(pVTable[8]);
            
            // Replace with our hook
            DWORD oldProtect;
            VirtualProtect(&pVTable[8], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
            pVTable[8] = &hkPresent;
            VirtualProtect(&pVTable[8], sizeof(void*), oldProtect, &oldProtect);
        }
        
        // Cleanup
        if (context) context->Release();
        if (device) device->Release();
        if (swapChain) swapChain->Release();
        DestroyWindow(hwnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

    void D3D11::Unhook() {
        // Restore original vtable entry
        if (oPresent && g_pSwapChain) {
            void** pVTable = *reinterpret_cast<void***>(g_pSwapChain);
            DWORD oldProtect;
            VirtualProtect(&pVTable[8], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
            pVTable[8] = oPresent;
            VirtualProtect(&pVTable[8], sizeof(void*), oldProtect, &oldProtect);
        }
    }

    void Input::ProcessKeyInput(int key, bool isDown) {
        // Toggle menu with INSERT key
        if (key == VK_INSERT && isDown) {
            Menu::IsOpen = !Menu::IsOpen;
        }
    }

    void Initialize() {
        D3D11::Hook();
    }

    void Shutdown() {
        D3D11::Unhook();
        UI::Renderer::Shutdown();
    }
}
