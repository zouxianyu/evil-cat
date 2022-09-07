#include <memory>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "interface/player_health_interface.h"
#include "health_locker.h"
#include "controller/controller.h"

namespace Settings::HealthLocker {
    bool on = false;
}

HealthLocker &HealthLocker::getInstance() {
    static HealthLocker instance;
    return instance;
}

void HealthLocker::guiCallback() {
    static bool previous = false;
    if (Settings::HealthLocker::on && !previous) {
        Controller::getInstance().addFastLoopCallback(
                std::bind(&HealthLocker::fastLoopCallback, this)
        );
    }
    previous = Settings::HealthLocker::on;
}

bool HealthLocker::fastLoopCallback() {
    if (!Settings::HealthLocker::on) {
        return false;
    }
    auto localPlayer = std::dynamic_pointer_cast<PlayerHealthInterface>(
            Game::getInstance().getLocalPlayer()
    );
    localPlayer->setHealth(100);
    return true;
}

