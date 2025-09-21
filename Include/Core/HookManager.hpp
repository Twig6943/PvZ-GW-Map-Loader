#pragma once

#include <vector>

namespace Hooks {

void installHooks();
void uninstallHooks();

}

class Hook;

class HookManager {
public:
    void initialize();
    void uninitialize();

    void installHooks();
    void uninstallHooks();

    Hook* registerHook(uintptr_t address, void* detourFunction, void** originalFunction, bool enable);

public:
    static HookManager& getManager() {
        static HookManager s_hookManager;
        return s_hookManager;
    }

private:
    std::vector<Hook*> m_hooks;

};