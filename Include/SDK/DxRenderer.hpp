#pragma once

#include <Types.hpp>
#include <SDK/Offsets.hpp>

namespace fb {

static inline void* getHwnd() {
    return *reinterpret_cast<void**>(0x141C28E08);
}

class DxRenderer {
public:
    virtual ~DxRenderer();

    class Screen* getScreen() { return m_screen; }

    struct ID3D11Device* getDevice() { return m_device; }
    struct ID3D11DeviceContext* getDeviceContext() { return m_deviceContext; }

    static DxRenderer* getInstance() {
        return *reinterpret_cast<DxRenderer**>(offsets::g_dxRendererInstance);
    }

private:
    /* 0x08 */  char pad_0008[0x30];
    /* 0x38 */  class Screen* m_screen;
    /* 0x40 */  char pad_0040[0xC0];
    /* 0x100 */ struct ID3D11Device* m_device;
    /* 0x108 */ struct ID3D11DeviceContext* m_deviceContext;

};

class Screen {
public:
    struct IDXGISwapChain* getSwapChain() {
        return m_swapChain;
    }

private:
    char pad_0000[0xC0];
    u32 m_screenWidth;
    u32 m_screenHeight;
    u32 m_screenWidth2;
    u32 m_screenHeight2;
    char pad_00D0[0x20];
    struct IDXGISwapChain* m_swapChain;
};

}
