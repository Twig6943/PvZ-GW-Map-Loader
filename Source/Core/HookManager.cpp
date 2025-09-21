#include <Core/HookManager.hpp>

#include <Core/Application.hpp>
#include <Core/Renderer.hpp>
#include <Core/Memory.hpp>
#include <Core/Hook.hpp>
#include <Util.hpp>

#include <SDK/Math.hpp>
#include <SDK/Entities.hpp>
#include <SDK/Offsets.hpp>
#include <SDK/DxRenderer.hpp>

#include <MinHook.h>

using tHalfExtentHook = void(*)(void*, void*, void*, void*, fb::Vec4*, fb::Matrix4x4*);
tHalfExtentHook origHalfExtentHook;

void halfExtentHook(void* param_1, void* param_2, void* param_3, void* param_4, fb::Vec4* halfExtentData, fb::Matrix4x4* transform) {
    if (Application::getApp().getSettings().FreedomEnabled) {
        halfExtentData->zero();
    }

    return origHalfExtentHook(param_1, param_2, param_3, param_4, halfExtentData, transform);
}

using tUpdatePreRoundHook = void(*)(fb::ServerPVZPreRoundEntity*);
tUpdatePreRoundHook origUpdatePreRoundHook;

void updatePreRoundHook(fb::ServerPVZPreRoundEntity* thisp) {
    if (Application::getApp().getSettings().LowerPreRoundRequirement) {
        thisp->getData()->setRequiresOnePlayer();
    }

    return origUpdatePreRoundHook(thisp);
}

using tClientCtorHook = void(*)(void*, void*);
tClientCtorHook origClientCtorHook;
void clientCtorHook(void* thisp, void* param_2) {
    origClientCtorHook(thisp, param_2);

    Renderer::getInstance().initialize();
}

void Hooks::installHooks() {
    HookManager& hookMgr = HookManager::getManager();
    hookMgr.initialize();
    printf("Initialized MinHook.\n");

    hookMgr.registerHook(offsets::g_simplePhysicsEntityCtor, halfExtentHook, reinterpret_cast<void**>(&origHalfExtentHook), true);
    hookMgr.registerHook(offsets::g_preroundUpdate, updatePreRoundHook, reinterpret_cast<void**>(&origUpdatePreRoundHook), true);
    
    // If we're injecting via DLL proxy, we'll wait for fb::Client to initialize before starting the renderer
    if (!fb::DxRenderer::getInstance()) {
        hookMgr.registerHook(offsets::g_clientCtor, clientCtorHook, reinterpret_cast<void**>(&origClientCtorHook), true);
    }
    else {
        Renderer::getInstance().initialize();
    }
}

void Hooks::uninstallHooks() {
    // Update combat area patch
    Memory::patch(offsets::g_serverCombatAreaEntityUpdate, std::span<const u8> {
        { 0x40, 0x53 } // push rbx
    });

    // Update kill volume patch
    Memory::patch(offsets::g_killVolumeFunction, std::span<const u8> {
        { 0x40, 0x57 } // push rdi
    });

    Memory::patch(offsets::g_serverPlayerUpdateInactivity, std::span<const u8> {
        { 0x48, 0x8B, 0xC4 } // mov rax, rsp
    });

    HookManager::getManager().uninitialize();
}

void HookManager::initialize() {
    MH_STATUS initResult = MH_Initialize();
    if (initResult != MH_OK) {
        util::fatalErrorMessage("Failed to initialize MinHook.");
    }
}

void HookManager::uninitialize() {
    uninstallHooks();
    MH_Uninitialize();
}

void HookManager::installHooks() {
    for (auto* hook : m_hooks) {
        hook->install();
    }
}

void HookManager::uninstallHooks() {
    for (auto* hook : m_hooks) {
        hook->uninstall();
    }
}

Hook* HookManager::registerHook(uintptr_t address, void* detourFunction, void** originalFunction, bool enable) {
    Hook* newHook = new Hook(address, detourFunction, originalFunction);
    m_hooks.emplace_back(newHook);

    if (enable) {
        newHook->install();
    }

    return newHook;
}