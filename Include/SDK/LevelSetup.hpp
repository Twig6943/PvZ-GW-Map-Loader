#pragma once

#include "Containers.hpp"
#include "String.hpp"
#include "Types.hpp"

namespace fb {

struct LevelSetupOption {
    /* 0x00 */ String Criterion;
    /* 0x08 */ String Option;
}; /* Size: 0x10 */

struct LevelSetup {
    void setInclusionOption(const char* criterion, const char* value);
    void setInclusionOptions(const char* options);
    void setLevelName(const char* name);

    /* 0x00 */ String                  Name;
    /* 0x08 */ Array<LevelSetupOption> InclusionOptions;
    /* 0x10 */ u32                     DifficultyIndex;
    /* 0x18 */ Array<String>           SubLevelNames;
    /* 0x20 */ String                  StartPoint;
    /* 0x28 */ Array<s32>              SubLevelStates;
    /* 0x30 */ bool                    IsSaveGame;
    /* 0x31 */ bool                    ForceReloadResources;
}; /* Size: 0x38 */

} // namespace fb
