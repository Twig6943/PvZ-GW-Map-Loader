#include "SDK/LevelSetup.hpp"

#include <SDK/Offsets.hpp>

void fb::LevelSetup::setInclusionOption(const char* criterion, const char* value) {
    using tSetInclusionOption = void(*)(fb::LevelSetup*, const char*, const char*);
    auto pSetInclusionOption = reinterpret_cast<tSetInclusionOption>(offsets::g_setInclusionOption);

    pSetInclusionOption(this, criterion, value);
}

void fb::LevelSetup::setInclusionOptions(const char* options) {
    using tSetInclusionOptions = void(*)(fb::LevelSetup*, const char*);
    auto pSetInclusionOptions = reinterpret_cast<tSetInclusionOptions>(offsets::g_setInclusionOptions);

    pSetInclusionOptions(this, options);
}

void fb::LevelSetup::setLevelName(const char* name) {
    Name.set(name);
}
