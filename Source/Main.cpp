#include <Windows.h>
#include <stdio.h>
#include <utility>

#include <Core/Application.hpp>

DWORD WINAPI hackThread(LPVOID hInstance) {
    Application& app = Application::getApp();
    app.initialize();
    app.run();
    app.uninitialize();

    FreeLibraryAndExitThread(reinterpret_cast<HINSTANCE>(hInstance), 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved [[maybe_unused]]) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        HANDLE threadHandle = CreateThread(nullptr, 0, hackThread, hinstDLL, 0, nullptr);

        if (threadHandle) {
            CloseHandle(threadHandle);
        }
    }

    return TRUE;
}
