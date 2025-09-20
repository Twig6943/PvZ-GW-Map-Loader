#pragma once

#include <Types.hpp>

#define DEFINE_OFFSET(n, o) inline constexpr uintptr_t g_##n = o

namespace offsets {

DEFINE_OFFSET(serverCombatAreaEntityUpdate, 0x140781AC0);
DEFINE_OFFSET(simplePhysicsEntityCtor, 0x140865C30);
DEFINE_OFFSET(levelListReportingAsset, 0x141F0B5B0);
DEFINE_OFFSET(killVolumeFunction, 0x140771E80);
DEFINE_OFFSET(preroundUpdate, 0x140C07F40);
DEFINE_OFFSET(sendServerLoadLevelMessage, 0x1406F6FD0);
DEFINE_OFFSET(setStringFunction, 0x14036FFC0);
DEFINE_OFFSET(setInclusionOption, 0x1404FA7F0);
DEFINE_OFFSET(setInclusionOptions, 0x1404FA9F0);
DEFINE_OFFSET(dxRendererInstance, 0x141FEB230);
DEFINE_OFFSET(clientGameContextInstance, 0x141F000E0);
DEFINE_OFFSET(serverGameContextInstance, 0x141FC3180);

}
