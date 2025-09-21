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

tPresentHook oPresentHook;
HRESULT Renderer::presentHook(IDXGISwapChain* swapChain, u32 syncInterval, u32 flags) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (s_showMenu) {
        s_menu->drawMenu();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresentHook(swapChain, syncInterval, flags);
}

tResizeBuffers oResizeBuffers;
HRESULT Renderer::resizeBuffersHook(IDXGISwapChain* swapChain, u32 bufferCount, u32 width, u32 height, s32 newFormat, u32 swapChainFlags) {
    Renderer::getInstance().releaseRenderTargetView();
    HRESULT result = oResizeBuffers(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
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

    MH_CreateHook(m_presentAddress, presentHook, reinterpret_cast<LPVOID*>(&oPresentHook));
    MH_EnableHook(m_presentAddress);

    MH_CreateHook(m_resizeBuffersAddress, resizeBuffersHook, reinterpret_cast<LPVOID*>(&oResizeBuffers));
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
    shutdownImGui();
    releaseRenderTargetView();
}

void Renderer::initImGui() {
    ImGui_ImplWin32_EnableDpiAwareness();
    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(fb::getHwnd());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(dpiScale);
    style.FontScaleDpi = dpiScale;

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
