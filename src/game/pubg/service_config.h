#ifndef EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H
#define EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose your services

#include "service/service_container.h"

#include "service/aimbot/aimbot.h"
#include "service/esp/esp.h"
#include "service/radar/radar.h"
#include "service/name_list/name_list.h"

#include "settings.h"

using ServiceList = ServiceContainer<
        Aimbot,
        Esp,
//        Radar,
        NameList
>;

//////////////////////////////////////////////////////////////////////////
// change default settings

inline static void changeDefaultSettings() {
    Settings::distanceFactor = 0.01f;
    Settings::Esp::viewLineScale = 30.f;
    Settings::Esp::showBone = true;
    Settings::Radar::scale = 0.05f;
    Settings::NameList::on = true;
}

#endif //EVIL_CAT_GAME_PUBG_SERVICE_CONFIG_H
