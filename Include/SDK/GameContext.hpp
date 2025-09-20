#pragma once

#include "Types.hpp"
#include "Message.hpp"
#include "LevelSetup.hpp"
#include <SDK/Offsets.hpp>

namespace fb {

class GameTime {
public:

private:
    /* 0x00 */ u32   m_ticks;
    /* 0x04 */ u32   m_updateInterval;
    /* 0x08 */ u64   unk1;
    /* 0x10 */ bool  m_isUpdating;
    /* 0x14 */ float unk2;
}; /* Size: 0x18*/

class Level {
public:
    LevelSetup& getLevelSetup() { return m_levelSetup; }

private:
    /* 0x00 */ char       pad_0000[0x48];
    /* 0x48 */ LevelSetup m_levelSetup;

};

class GameContext {
public:
    static GameContext* getInstance() {
        return *reinterpret_cast<GameContext**>(offsets::g_clientGameContextInstance);
    }

    Level* getLevel() const { return m_level; }

private:
    /* 0x00 */ void*                unk1;
    /* 0x08 */ MessageManager       m_messageManager;
    /* 0x18 */ class PlayerManager* m_playerManager;
    /* 0x20 */ GameTime*            m_gameTime;
    /* 0x28 */ Level*               m_level;
};
static_assert(sizeof(GameContext) == 0x30);

class ServerGameContext {
public:
    static ServerGameContext* getInstance() {
        return *reinterpret_cast<ServerGameContext**>(offsets::g_serverGameContextInstance);
    }
};

}
