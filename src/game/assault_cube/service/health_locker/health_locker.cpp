#include <memory>
#include "game.h"
#include "interface/player_health_interface.h"
#include "health_locker.h"
#include "controller/controller.h"

namespace Settings::HealthLocker {
    bool on = false;
}

void HealthLocker::callback() {
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
    std::shared_ptr<PlayerHealthInterface> localPlayer = Game::getInstance().getLocalPlayer();
    localPlayer->setHealth(100);
    return true;
}

