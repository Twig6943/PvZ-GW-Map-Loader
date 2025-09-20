#include <Util.hpp>

#include <Windows.h>
#include <cstdlib>

void util::fatalErrorMessage(const char* message) {
    MessageBoxA(nullptr, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
    std::exit(EXIT_FAILURE);
}
