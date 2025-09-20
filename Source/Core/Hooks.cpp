#include <Core/Hooks.hpp>

#include <Core/Memory.hpp>
#include <MinHook.h>
#include <SDK/Math.hpp>
#include <SDK/Entities.hpp>

#include <SDK/Offsets.hpp>

using tHalfExtentHook = void(*)(void*, void*, void*, void*, fb::Vec4*, fb::Matrix4x4*);
tHalfExtentHook oHalfExtentHook;

void halfExtentHook(void* param_1, void* param_2, void* param_3, void* param_4, fb::Vec4* halfExtentData, fb::Matrix4x4* transform) {
    halfExtentData->zero();

    return oHalfExtentHook(param_1, param_2, param_3, param_4, halfExtentData, transform);
}

using tUpdatePreRoundHook = void(*)(fb::ServerPVZPreRoundEntity*);
tUpdatePreRoundHook oUpdatePreRoundHook;

void updatePreRoundHook(fb::ServerPVZPreRoundEntity* thisp) {
    thisp->getData()->setRequiresOnePlayer();

    return oUpdatePreRoundHook(thisp);
}

void Hooks::installHooks() {
    // Update combat area patch
    Memory::patch(offsets::g_serverCombatAreaEntityUpdate, std::span<const u8> {
        { 0xC3 } // ret
    });

    // Update kill volume patch
    Memory::patch(offsets::g_killVolumeFunction, std::span<const u8> {
        { 0xC3 } // ret
    });

    MH_Initialize();
    MH_CreateHook(reinterpret_cast<LPVOID>(offsets::g_simplePhysicsEntityCtor), halfExtentHook, reinterpret_cast<LPVOID*>(&oHalfExtentHook));
    MH_EnableHook(reinterpret_cast<LPVOID>(offsets::g_simplePhysicsEntityCtor));

    MH_CreateHook(reinterpret_cast<LPVOID>(offsets::g_preroundUpdate), updatePreRoundHook, reinterpret_cast<LPVOID*>(&oUpdatePreRoundHook));
    MH_EnableHook(reinterpret_cast<LPVOID>(offsets::g_preroundUpdate));
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

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}
