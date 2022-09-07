#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H

namespace Settings::HealthLocker {
    extern bool on;
}

class HealthLocker {
public:
    static HealthLocker &getInstance();

    void guiCallback();
    bool fastLoopCallback();
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
