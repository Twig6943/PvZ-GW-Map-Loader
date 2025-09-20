#include <Core/Application.hpp>

#include <Core/Hooks.hpp>
#include <Core/Renderer.hpp>

#include <Windows.h>

void Application::initialize() {
    AllocConsole();
    SetConsoleTitleA(s_consoleTitle);

    Hooks::installHooks();
    Renderer::getInstance().initialize();
}

void Application::uninitialize() {
    Renderer::getInstance().shutdown();
    Hooks::uninstallHooks();
    FreeConsole();
}

void Application::run() {
    while (m_running) {
        if (GetAsyncKeyState(VK_F12) & 1) {
            m_running = false;
        }
    }
}
