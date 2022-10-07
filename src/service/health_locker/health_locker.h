#ifndef EVIL_CAT_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
#define EVIL_CAT_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H

#include "service/service_interface.h"

namespace Settings::HealthLocker {
    extern bool on;
    extern float lockHealth;
}

class HealthLocker : public ServiceInterface {
    bool fastLoopCallback();

public:
    void callback() override;

};

#endif //EVIL_CAT_SERVICE_HEALTH_LOCKER_HEALTH_LOCKER_H
