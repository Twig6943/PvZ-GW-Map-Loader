#include <Core/Application.hpp>

#include <stdio.h>
#include <Windows.h>
#include <Util.hpp>

/* 
    We can have the DLL load with game by renaming it to dinput8.dll and putting it in the game's install directory.
    To achieve this, we must export DirectInput8Create from the DLL and call the original function (see DllProxy.def).
*/
extern "C" HRESULT __stdcall proxyDirectInput8Create(
    HINSTANCE hinst, 
    DWORD dwVersion, 
    REFIID riidltf, 
    LPVOID* ppvOut, 
    LPUNKNOWN punkOuter) {
    
    using tDirectInput8Create = HRESULT(*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
    auto pDirectInput8Create = reinterpret_cast<tDirectInput8Create>(util::getDllExport("DirectInput8Create", "dinput8.dll"));
    if (!pDirectInput8Create) {
        util::fatalErrorMessage("Cannot run the program in DLL Proxy mode. Missing dinput8.dll");
        return E_FAIL;
    }

    return pDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

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
