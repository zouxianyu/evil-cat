#include <vector>
#include <functional>
#include <memory>
#include "game.h"
#include "menu.h"
#include "init.h"
#include "controller/controller.h"
#include "service/name_list/name_list.h"
#include "service/health_locker/health_locker.h"
#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"

std::shared_ptr<InitConfig> init() {
    std::vector<std::function<void()>> guiCallbacks;
    std::vector<std::function<bool()>> fastLoopCallbacks;

    // initialize your callbacks here

    // custom service : show name list on the left-top corner
    guiCallbacks.emplace_back(
            std::bind(&NameList::callback, &NameList::getInstance())
    );

    // core service : aimbot
    guiCallbacks.emplace_back(
            std::bind(&Aimbot::callback, &Aimbot::getInstance())
    );

    // core service : esp
    guiCallbacks.emplace_back(
            std::bind(&Esp::callback, &Esp::getInstance())
    );

    // try to lock local player's health value
    guiCallbacks.emplace_back(
            std::bind(&HealthLocker::guiCallback, &HealthLocker::getInstance())
    );

    // construct initialization config and return it to the caller (framework)
    std::shared_ptr<InitConfig> config = std::make_shared<InitConfig>(
            guiCallbacks,
            fastLoopCallbacks
    );
    return config;
}
