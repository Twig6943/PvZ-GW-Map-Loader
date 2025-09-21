#include <Core/Menu.hpp>

#include <SDK/GameContext.hpp>
#include <SDK/Assets.hpp>
#include <SDK/Util.hpp>
#include <SDK/Offsets.hpp>

#include <imgui.h>

#include <Core/Application.hpp>
#include <Core/Memory.hpp>

#include <functional>
#include <sstream>

inline void ImGuiButton(const char* label, const std::function<void()>& callback) {
    if (ImGui::Button(label)) {
        callback();
    }
}

void createImGuiComboIteration(const char** preview, const char* value) {
    bool isItemSelected = *preview == value;
    if (ImGui::Selectable(value, isItemSelected)) {
        *preview = value;
    }
    if (isItemSelected) {
        ImGui::SetItemDefaultFocus();
    }
}

void Menu::sendLoadLevelMessage() {
    if (!m_selectedLevel) {
        return;
    }
    if (!m_selectedGameMode) {
        return;
    }
    if (m_currentLevelHasTod && !m_selectedTod) {
        return;
    }
    if (m_currentLevelHasDebugMode && !m_selectedDebugMode && m_useDebugMode) {
        return;
    }

    fb::LevelSetup& setup = fb::GameContext::getInstance()->getLevel()->getLevelSetup();

    setup.setLevelName(m_selectedLevel);

    std::stringstream inclusionOptionsStream;
    inclusionOptionsStream << "GameMode=" << m_selectedGameMode;

    if (m_currentLevelHasTod) {
        inclusionOptionsStream << ";TOD=" << m_selectedTod;
    }

    if (m_currentLevelHasDebugMode && m_useDebugMode) {
        inclusionOptionsStream << ";ZDebugMode=" << m_selectedDebugMode;
    }

    setup.setInclusionOptions(inclusionOptionsStream.str().c_str());
    setup.DifficultyIndex = fbutil::GameDifficulty_Multiplayer;

    using tSendLoadLevelMessage = void(*)(fb::LevelSetup*, bool, bool, bool);
    auto pSendLoadLevelMessage = reinterpret_cast<tSendLoadLevelMessage>(offsets::g_sendServerLoadLevelMessage);
    pSendLoadLevelMessage(&setup, true, true, false);
}

void Menu::drawLevelOptionsSelection() {
    fb::LevelReportingAsset* reportingAsset = *reinterpret_cast<fb::LevelReportingAsset**>(0x141F0B5B0);
    if (!reportingAsset) {
        return;
    }

    if (ImGui::BeginCombo("Level", m_selectedLevel)) {
        for (auto* descriptions : reportingAsset->getBuiltLevels()) {
            createImGuiComboIteration(&m_selectedLevel, descriptions->getLevelName().asCString());
        }
        ImGui::EndCombo();
    }

    if (!m_selectedLevel) {
        return;
    }

    fb::LevelDescriptionAsset* descriptionAsset = reportingAsset->getLevelDescription(m_selectedLevel);
    if (!descriptionAsset) {
        return;
    }

    if (ImGui::BeginCombo("GameMode", m_selectedGameMode)) {
        for (auto& mode : descriptionAsset->getCategory("GameMode")->Mode) {
            createImGuiComboIteration(&m_selectedGameMode, mode.asCString());
        }

        ImGui::EndCombo();
    }

    m_currentLevelHasTod = descriptionAsset->hasCategory("TOD");
    if (m_currentLevelHasTod) {
        if (ImGui::BeginCombo("TOD", m_selectedTod)) {
            for (auto& option : descriptionAsset->getCategory("TOD")->Mode) {
                createImGuiComboIteration(&m_selectedTod, option.asCString());
            }

            ImGui::EndCombo();
        }
    }

    m_currentLevelHasDebugMode = descriptionAsset->hasCategory("ZDebugMode");
    if (m_currentLevelHasDebugMode) {
        if (ImGui::BeginCombo("ZDebugMode", m_selectedDebugMode)) {
            for (auto& option : descriptionAsset->getCategory("ZDebugMode")->Mode) {
                createImGuiComboIteration(&m_selectedDebugMode, option.asCString());
            }

            ImGui::EndCombo();
        }

        ImGui::SameLine();
        
        ImGui::Checkbox("Use", &m_useDebugMode);
    }
}

void Menu::drawMenu() {
    ImGui::Begin("PvZ GW Map Loader");

    // "Fix" until I figure out how to allow for level loading in the front end.
    if (fbutil::isLocalPlayerInFrontEnd() || !fbutil::isLocalPlayerServerHost()) {
        ImGui::Text("Level loading is not supported in the Main Menu.");
        ImGui::Text("Press INSERT to toggle UI visiblity.");
        ImGui::End();
        return;
    }

    drawLevelOptionsSelection();

    ImGuiButton("Load Level", 
        [this] {
            if (!fbutil::isLocalPlayerInFrontEnd() && fbutil::isLocalPlayerServerHost()) {
                sendLoadLevelMessage();
            }
        }
    );

    ApplicationSettings& appSettings = Application::getApp().getSettings();

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
    
    ImGui::Text("Press INSERT to toggle UI visiblity.");

    ImGui::End();
}
