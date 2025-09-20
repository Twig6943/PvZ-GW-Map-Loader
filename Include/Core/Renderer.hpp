#pragma once

#include <Types.hpp>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

typedef long HRESULT;

class Renderer {
public:
    void initialize();
    void shutdown();

public:
    static Renderer& getInstance() {
        static Renderer s_renderer;
        return s_renderer;
    }

private:
    void populateDxgiFunctionPointers();

    void initImGui();
    void shutdownImGui();

    void createRenderTargetView();
    void releaseRenderTargetView();

private:
    static HRESULT presentHook(IDXGISwapChain* swapChain, u32 syncInterval, u32 flags);
    static HRESULT resizeBuffersHook(IDXGISwapChain* swapChain, u32 bufferCount, u32 width, u32 height, s32 newFormat, u32 swapChainFlags);

private:
    ID3D11Device*           m_device = nullptr;
    ID3D11DeviceContext*    m_deviceContext = nullptr;
    IDXGISwapChain*         m_swapChain = nullptr;
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
    void*                   m_presentAddress = nullptr;
    void*                   m_resizeBuffersAddress = nullptr;

};
