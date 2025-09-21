#include <Core/Renderer.hpp>

#include <Types.hpp>
#include <Core/Menu.hpp>
#include <Util.hpp>

#include <SDK/DxRenderer.hpp>

#include <d3d11.h>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

static Menu* s_menu = nullptr;
static bool s_showMenu = true;

using tPresentHook = HRESULT(*)(IDXGISwapChain*, u32, u32);
using tResizeBuffers = HRESULT(*)(IDXGISwapChain*, u32, u32, u32, s32, u32);

tPresentHook origPresentHook;
HRESULT Renderer::presentHook(IDXGISwapChain* swapChain, u32 syncInterval, u32 flags) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (s_showMenu) {
        s_menu->drawMenu();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return origPresentHook(swapChain, syncInterval, flags);
}

tResizeBuffers origResizeBuffers;
HRESULT Renderer::resizeBuffersHook(IDXGISwapChain* swapChain, u32 bufferCount, u32 width, u32 height, s32 newFormat, u32 swapChainFlags) {
    // Game will crash upon resize if not handled. All we have to do is free then recreate the render target view.
    Renderer::getInstance().releaseRenderTargetView();
    HRESULT result = origResizeBuffers(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
    Renderer::getInstance().createRenderTargetView();
    return result;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK* tWndProc)(HWND, UINT, WPARAM, LPARAM);
tWndProc oWndProc;
LRESULT windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN) {
        // Insert key and not down on previous message
        if (wParam == VK_INSERT && !(lParam & (1 << 30))) {
            s_showMenu = !s_showMenu;
        }
    }

    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
        return true;
    }

    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

void Renderer::initialize() {
    fb::DxRenderer* dxRenderer = fb::DxRenderer::getInstance();
    m_device = dxRenderer->getDevice();
    m_deviceContext = dxRenderer->getDeviceContext();
    
    fb::Screen* screen = dxRenderer->getScreen();
    m_swapChain = screen->getSwapChain();

    populateDxgiFunctionPointers();

    s_menu = new Menu();

    initImGui();

    oWndProc = reinterpret_cast<tWndProc>(
        SetWindowLongPtr(reinterpret_cast<HWND>(fb::getHwnd()), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(windowProcedure))
    );

    MH_CreateHook(m_presentAddress, presentHook, reinterpret_cast<LPVOID*>(&origPresentHook));
    MH_EnableHook(m_presentAddress);

    MH_CreateHook(m_resizeBuffersAddress, resizeBuffersHook, reinterpret_cast<LPVOID*>(&origResizeBuffers));
    MH_EnableHook(m_resizeBuffersAddress);
}

void Renderer::populateDxgiFunctionPointers() {
    void** swapChainVtable = *reinterpret_cast<void***>(m_swapChain);
    m_presentAddress = swapChainVtable[8];
    m_resizeBuffersAddress = swapChainVtable[13];

    if (!m_presentAddress || !m_resizeBuffersAddress) {
        util::fatalErrorMessage("Failed to get IDXGISwapChain method pointers.");
    }
}

void Renderer::shutdown() {
    MH_DisableHook(m_presentAddress);
    MH_DisableHook(m_resizeBuffersAddress);

    shutdownImGui();
    releaseRenderTargetView();
}

void Renderer::initImGui() {
    /*
        Window sizing will look weird on screens with different DPIs. We can scale everything by the DPI scale to achieve a
        consistent look
    */
    ImGui_ImplWin32_EnableDpiAwareness();
    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(fb::getHwnd());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange; // Will use default Windows cursor otherwise.
    
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(dpiScale);
    style.FontScaleDpi = dpiScale;

    // Styling
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 2.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

    style.Colors[ImGuiCol_Button] = ImColor(95, 43, 135, 255).Value;
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(154, 66, 250, 204).Value;
    style.Colors[ImGuiCol_ButtonActive] = ImColor(143, 80, 193, 255).Value;
    style.Colors[ImGuiCol_CheckMark] = ImColor(178, 104, 200, 255).Value;
    style.Colors[ImGuiCol_FrameBg] = ImColor(48, 48, 48, 138).Value;
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(100, 100, 100, 171).Value;
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(100, 100, 100, 102).Value;
    style.Colors[ImGuiCol_ResizeGrip] = ImColor(154, 41, 227, 108).Value;
    style.Colors[ImGuiCol_SliderGrab] = ImColor(139, 61, 224, 255).Value;
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(172, 77, 250, 255).Value;
    style.Colors[ImGuiCol_Tab] = ImColor(116, 67, 155, 220).Value;
    style.Colors[ImGuiCol_TabHovered] = ImColor(154, 66, 250, 204).Value;
    style.Colors[ImGuiCol_TabSelected] = ImColor(143, 80, 193, 255).Value;
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(116, 67, 155, 255).Value;
    style.Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255).Value;

    ImGui_ImplWin32_Init(fb::getHwnd());
    ImGui_ImplDX11_Init(m_device, m_deviceContext);
}

void Renderer::shutdownImGui() {
    MH_DisableHook(m_presentAddress);
    MH_DisableHook(m_resizeBuffersAddress);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::createRenderTargetView() {
    ID3D11Texture2D* backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();
}

void Renderer::releaseRenderTargetView() {
    if (!m_renderTargetView) {
        return;
    }

    m_renderTargetView->Release();
    m_renderTargetView = nullptr;
}
