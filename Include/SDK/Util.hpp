#pragma once

namespace fbutil {

// Derived from difficulty data found in _pvz/GameConfigurations/Game_Difficulty_Settings
enum GameDifficulty : int {
    GameDifficulty_Multiplayer  = 0,
    GameDifficulty_Easy         = 1,
    GameDifficulty_Normal       = 2,
    GameDifficulty_Hard         = 3,
    GameDifficulty_Crazy        = 4,
    GameDifficulty_SinglePlayer = 5,
};

bool isLocalPlayerServerHost();
bool isLocalPlayerInFrontEnd();

}
