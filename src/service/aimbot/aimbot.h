#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "service/service_interface.h"
#include "game/interface/player_basic_interface.h"

namespace Settings::Aimbot {
    extern bool on;
    extern float maxAngle;
    enum class Strategy : int {
        rightButtonPrecise,
        rightButtonSmooth,
        autoFollowPrecise,
        autoFollowSmooth
    };
    extern Strategy strategy;
}

class Aimbot : public ServiceInterface {
    std::optional<std::shared_ptr<PlayerBasicInterface>> aimbotTarget = std::nullopt;

public:
    void callback();

};

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
