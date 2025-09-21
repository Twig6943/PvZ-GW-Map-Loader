#pragma once

#include <Types.hpp>

class Hook {
public:
    Hook(uintptr_t address, void* detourFunction, void** originalFunction)
        : m_address(address), m_detourFunction(detourFunction), m_originalFunction(originalFunction),
          m_lastError(0), m_installed(false) {}

    void install();
    void uninstall();

private:
    uintptr_t m_address;
    void* m_detourFunction;
    void** m_originalFunction;
    s32 m_lastError;
    bool m_installed;

};