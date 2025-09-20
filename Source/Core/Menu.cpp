#include <Core/Menu.hpp>

#include <SDK/GameContext.hpp>
#include <SDK/Util.hpp>
#include <SDK/Offsets.hpp>

#include <imgui.h>

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

    if (ImGui::Button("Load Level")) {
        if (!fbutil::isLocalPlayerInFrontEnd() && fbutil::isLocalPlayerServerHost()) {
            sendLoadLevelMessage();
        }
    }
    
    ImGui::End();
}
