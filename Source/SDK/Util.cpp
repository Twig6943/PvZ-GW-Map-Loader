#include <SDK/Util.hpp>

#include <SDK/GameContext.hpp>

bool fbutil::isLocalPlayerServerHost() {
    // ServerGameContext instance only exists while the host.
    return fb::ServerGameContext::getInstance() != nullptr;
}

bool fbutil::isLocalPlayerInFrontEnd() {
    fb::GameContext* gameCtx = fb::GameContext::getInstance();
    if (!gameCtx) [[unlikely]] {
        return true;
    }

    fb::Level* level = gameCtx->getLevel();
    if (!level) {
        return true;
    }

    fb::LevelSetup& levelSetup = level->getLevelSetup();
    return levelSetup.Name.equals("_pvz/Levels/Level_Frontend_Gen3/Level_Frontend_Gen3");
}
