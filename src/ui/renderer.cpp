#include "renderer.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace UI::Renderer {
    static ID3D11Device* g_pd3dDevice = nullptr;
    static ID3D11DeviceContext* g_pd3dContext = nullptr;
    static HWND g_hwnd = nullptr;
    static bool g_initialized = false;

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd) {
        g_pd3dDevice = device;
        g_pd3dContext = context;
        g_hwnd = hwnd;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(g_hwnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);

        g_initialized = true;
        return true;
    }

    void Shutdown() {
        if (g_initialized) {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
            g_initialized = false;
        }
    }

    void NewFrame() {
        if (!g_initialized) return;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame(ID3D11RenderTargetView* rtv) {
        if (!g_initialized) return;

        ImGui::Render();
        g_pd3dContext->OMSetRenderTargets(1, &rtv, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    bool IsReady() {
        return g_initialized;
    }

    void DrawText(float x, float y, const char* text, ImColor color, float size) {
        if (!g_initialized) return;
        
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddText(nullptr, size, ImVec2(x, y), color, text);
    }

    void DrawBox(float x, float y, float w, float h, ImColor color, float thickness) {
        if (!g_initialized) return;
        
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f, ImDrawFlags_None, thickness);
    }

    void DrawLine(float x1, float y1, float x2, float y2, ImColor color, float thickness) {
        if (!g_initialized) return;
        
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
    }
}
