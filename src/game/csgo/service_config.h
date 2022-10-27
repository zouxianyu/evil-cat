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

#endif //EVIL_CAT_GAME_CSGO_SERVICE_CONFIG_H
