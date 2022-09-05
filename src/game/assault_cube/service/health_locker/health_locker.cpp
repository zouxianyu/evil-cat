#include <memory>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "interface/player_health_interface.h"
#include "health_locker.h"

HealthLocker &HealthLocker::getInstance() {
    static HealthLocker instance;
    return instance;
}

bool HealthLocker::callback() {
    auto localPlayer = std::dynamic_pointer_cast<PlayerHealthInterface>(
            Game::getInstance().getLocalPlayer()
    );
    localPlayer->setHealth(100);
    return true;
}

