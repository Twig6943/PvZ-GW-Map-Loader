#include <Core/Application.hpp>

#include <Core/HookManager.hpp>
#include <Core/Renderer.hpp>

#include <Windows.h>

void Application::initialize() {
    AllocConsole();
    SetConsoleTitleA(s_consoleTitle);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    Hooks::installHooks();
}

void Application::uninitialize() {
    // TODO: Game currently crashes during uninitialization.
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
