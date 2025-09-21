#include <Core/Hook.hpp>

#include <Util.hpp>
#include <MinHook.h>

void Hook::install() {
    if (m_installed) {
        return;
    }

    m_lastError = MH_CreateHook(reinterpret_cast<LPVOID>(m_address), m_detourFunction, m_originalFunction);
    if (m_lastError != MH_OK) {
        util::fatalErrorMessage("Failed to create hook.");
    }

    m_lastError = MH_EnableHook(reinterpret_cast<LPVOID>(m_address));
    if (m_lastError != MH_OK) {
        util::fatalErrorMessage("Failed to enable hook.");
    }
}

void Hook::uninstall() {
    if (!m_installed) {
        return;
    }

    m_lastError = MH_DisableHook(reinterpret_cast<LPVOID>(m_address));
    if (m_lastError != MH_OK) {
        util::fatalErrorMessage("Failed to disable hook.");
    }

    m_lastError = MH_RemoveHook(reinterpret_cast<LPVOID>(m_address));
    if (m_lastError != MH_OK) {
        util::fatalErrorMessage("Failed to remove hook.");
    }
}