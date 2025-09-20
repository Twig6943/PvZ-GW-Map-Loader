#pragma once

#include <Types.hpp>

namespace fb {

class PVZPreRoundEntityData {
public:
    void setRequiresOnePlayer() {
        m_roundStartPlayerCount = 0;
        m_roundRestartCountdown = 0;
        m_requireSpawnedPlayers = true;
    }

private:
    /* 0x00 */ char pad_0000[0x18];
    /* 0x18 */ s32 m_roundStartPlayerCount;
    /* 0x1C */ u32 m_roundStartCountdown;
    /* 0x20 */ s32 m_roundRestartPlayerCount;
    /* 0x24 */ u32 m_roundRestartCountdown;
    /* 0x28 */ bool m_requireSpawnedPlayers;
};

class ServerPVZPreRoundEntity {
public:
    PVZPreRoundEntityData* getData() { return m_data; }

private:
    /* 0x00 */ char pad_0000[0x28];
    /* 0x28 */ PVZPreRoundEntityData* m_data;
};

} // namespace fb
