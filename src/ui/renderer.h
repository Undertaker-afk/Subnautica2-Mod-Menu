#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>

namespace UI {
    namespace Renderer {
        bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd);
        void Shutdown();
        void NewFrame();
        void EndFrame(ID3D11RenderTargetView* rtv);
        bool IsReady();
        
        // Drawing helpers
        void DrawText(float x, float y, const char* text, ImColor color, float size);
        void DrawBox(float x, float y, float w, float h, ImColor color, float thickness);
        void DrawLine(float x1, float y1, float x2, float y2, ImColor color, float thickness);
    }
}
