#ifndef EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H
#define EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose your services

#include "service/service_container.h"

#include "service/aimbot/aimbot.h"
#include "service/aimbot/aimbot_strategy.h"
#include "service/item_esp/item_esp.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"
#include "service/name_list/name_list.h"

#include "settings.h"

using ServiceList = ServiceContainer<
        Aimbot,
        ItemEsp,
        Esp,
        Radar
>;

//////////////////////////////////////////////////////////////////////////
// change default settings

inline static void changeDefaultSettings() {
    Settings::distanceFactor = 0.01f;
    Settings::Aimbot::strategy.aimer = AimbotStrategy::predictAimer;
    Settings::Aimbot::bone = Bone::spine;
    Settings::Esp::showBone = true;
    Settings::ItemEsp::level = 4;
    Settings::Radar::scale = 0.01f;
}

#endif //EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H
