#include <Core/Memory.hpp>

#include <Windows.h>

void Memory::patch(uintptr_t address, std::span<const u8> bytes) {
    DWORD oldProtect;
    VirtualProtect(reinterpret_cast<void*>(address), bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(reinterpret_cast<void*>(address), bytes.data(), bytes.size());
    VirtualProtect(reinterpret_cast<void*>(address), bytes.size(), oldProtect, &oldProtect);
}
