#include <Core/Application.hpp>

#include <Core/HookManager.hpp>
#include <Core/Renderer.hpp>

#include <Windows.h>

#include <SDK/Assets.hpp>

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
        if (GetAsyncKeyState(VK_F1) & 1) {
            auto levelReporting = *reinterpret_cast<fb::LevelReportingAsset**>(0x141F0B5B0);
            printf("%s\nSize: %d\n", levelReporting->getName().asCString(), levelReporting->getBuiltLevels().size());

            auto descriptions = levelReporting->getBuiltLevels();
            for (auto* description : descriptions) {
                printf("%s\n", description->getLevelName().asCString());
                
                printf("\tCategories:\n");
                for (auto& category : description->getCategories()) {
                    printf("\t\t%s\n", category.Category.asCString());
                    for (auto& mode : category.Mode) {
                        printf("\t\t\t%s\n", mode.asCString());
                    }

                }
                printf("\n");
            }
        }
        if (GetAsyncKeyState(VK_F12) & 1) {
            m_running = false;
        }
    }
}
