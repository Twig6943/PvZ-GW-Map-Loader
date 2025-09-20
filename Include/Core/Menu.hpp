#pragma once

class Menu {
public:
    void drawMenu();

private:
    void sendLoadLevelMessage();

private:
    char m_levelInputText[256];
    char m_inclusionOptionsInputText[256];

};
