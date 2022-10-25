#include <memory>
#include "module.h"
#include "controller/controller.h"
#include "health_locker.h"

namespace Settings::HealthLocker {
    bool on = false;
    float lockHealth = 100.f;
}

std::string HealthLocker::getName() {
    return "health locker";
}

void HealthLocker::menuCallback() {

}

void HealthLocker::serviceCallback() {
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
    Module::game->getLocalPlayer()->setHealth(Settings::HealthLocker::lockHealth);
    return true;
}

