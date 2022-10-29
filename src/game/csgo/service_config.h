#ifndef EVIL_CAT_GAME_CSGO_SERVICE_CONFIG_H
#define EVIL_CAT_GAME_CSGO_SERVICE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose your services

#include <tuple> // type list container

#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"

using ServicesTypeList = std::tuple<
        Aimbot,
        Esp,
        Radar
>;

//////////////////////////////////////////////////////////////////////////
// change defalut settings

inline static void changeDefaultSettings() {
    Settings::Esp::viewLineScale = 30.f;
    Settings::Radar::scale = 0.05f;
}

#endif //EVIL_CAT_GAME_CSGO_SERVICE_CONFIG_H
