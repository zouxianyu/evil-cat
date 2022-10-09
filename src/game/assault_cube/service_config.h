#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose your services

#include <tuple> // type list container

#include "service/name_list/name_list.h"
#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"
#include "service/health_locker/health_locker.h"

using ServicesTypeList = std::tuple<
        NameList,
        Aimbot,
        Esp,
        Radar,
        HealthLocker
>;

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H
