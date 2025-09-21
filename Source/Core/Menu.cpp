#include <Core/Menu.hpp>

#include <SDK/GameContext.hpp>
#include <SDK/Util.hpp>
#include <SDK/Offsets.hpp>

#include <imgui.h>

#include <Core/Application.hpp>
#include <Core/Memory.hpp>

#include <functional>

inline void ImGuiButton(const char* label, const std::function<void()>& callback) {
    if (ImGui::Button(label)) {
        callback();
    }
}

void Menu::sendLoadLevelMessage() {
    fb::LevelSetup& setup = fb::GameContext::getInstance()->getLevel()->getLevelSetup();

    // TODO: validity checks or do away with letting the user supply their own input
    setup.setLevelName(m_levelInputText);
    setup.setInclusionOptions(m_inclusionOptionsInputText);
    setup.DifficultyIndex = fbutil::GameDifficulty_Multiplayer;

    using tSendLoadLevelMessage = void(*)(fb::LevelSetup*, bool, bool, bool);
    auto pSendLoadLevelMessage = reinterpret_cast<tSendLoadLevelMessage>(offsets::g_sendServerLoadLevelMessage);
    pSendLoadLevelMessage(&setup, true, true, false);
}

void Menu::drawMenu() {
    ImGui::Begin("PvZ GW Map Loader");

    ImGui::InputText("Level Name", &m_levelInputText[0], sizeof(m_levelInputText));
    ImGui::InputText("Inclusion Options", &m_inclusionOptionsInputText[0], sizeof(m_inclusionOptionsInputText));

    ApplicationSettings& appSettings = Application::getApp().getSettings();

    ImGuiButton("Load Level", 
        [this] {
            if (!fbutil::isLocalPlayerInFrontEnd() && fbutil::isLocalPlayerServerHost()) {
                sendLoadLevelMessage();
            }
        }
    );

    if (ImGui::Checkbox("Freedom", &appSettings.FreedomEnabled)) {
        if (appSettings.FreedomEnabled) {
            Memory::patch(offsets::g_serverCombatAreaEntityUpdate, std::span<const u8> {
                { 0xC3 } // ret
            });
            Memory::patch(offsets::g_killVolumeFunction, std::span<const u8> {
                { 0xC3 } // ret
            });
        }
        else {
            Memory::patch(offsets::g_serverCombatAreaEntityUpdate, std::span<const u8> {
                { 0x40, 0x53 } // push rbx
            });
            Memory::patch(offsets::g_killVolumeFunction, std::span<const u8> {
                { 0x40, 0x57 } // push rdi
            });
        }
    }

    ImGui::SameLine();
    
    ImGui::Checkbox("Lower Pre-Round Requirement", &appSettings.LowerPreRoundRequirement);

    ImGui::SameLine();

    if (ImGui::Checkbox("No Idle Kick", &appSettings.DisableIdleKick)) {
        if (appSettings.DisableIdleKick) {
            Memory::patch(offsets::g_serverPlayerUpdateInactivity, std::span<const u8> {
                { 0xC3 } // ret
            });
        }
        else {
            Memory::patch(offsets::g_serverPlayerUpdateInactivity, std::span<const u8> {
                { 0x48, 0x8B, 0xC4 } // mov rax, rsp
            });
        }
    }
    
    ImGui::End();
}
