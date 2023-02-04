#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose your services

#include "service/service_container.h"

#include "service/name_list/name_list.h"
#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"
#include "service/health_locker/health_locker.h"

#include "settings.h"

using ServiceList = ServiceContainer<
        NameList,
        Aimbot,
        Esp,
        Radar,
        HealthLocker
>;

//////////////////////////////////////////////////////////////////////////
// change defalut settings

inline static void changeDefaultSettings() {
    Settings::distanceFactor = 0.3f;
}

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_CONFIG_H
