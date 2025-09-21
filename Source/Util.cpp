#include <Util.hpp>

#include <Windows.h>
#include <cstdlib>

void util::fatalErrorMessage(const char* message) {
    MessageBoxA(nullptr, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
    std::exit(EXIT_FAILURE);
}

void* util::getDllExport(const char* symbol, const char* dllName) {
    char dllPath[MAX_PATH];
    GetSystemDirectoryA(dllPath, sizeof(dllPath));
    strcat_s(dllPath, "/");
    strcat_s(dllPath, dllName);

    HMODULE lib = LoadLibraryA(dllPath);
    if (!lib) {
        return nullptr;
    }

    void* address = GetProcAddress(lib, symbol);
    if (!address) {
        FreeLibrary(lib);
        return nullptr;
    }

    return address;
}