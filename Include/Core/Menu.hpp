#pragma once

class Menu {
public:
    void drawMenu();

private:
    void drawLevelOptionsSelection();
    void sendLoadLevelMessage();

    void onLevelChanged();

private:
    const char* m_selectedLevel = nullptr;
    const char* m_selectedGameMode = nullptr;
    const char* m_selectedTod = nullptr;
    const char* m_selectedDebugMode = nullptr;
    bool m_currentLevelHasTod = false;
    bool m_currentLevelHasDebugMode = false;
    bool m_useDebugMode = true;

};
