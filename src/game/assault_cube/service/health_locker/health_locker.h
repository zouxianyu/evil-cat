#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H

namespace Settings::HealthLocker {
    bool on = false;
}

class HealthLocker {
public:
    static HealthLocker &getInstance();

    void guiCallback();
    bool fastLoopCallback();
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
