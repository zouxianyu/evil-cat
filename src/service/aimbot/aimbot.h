#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
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

class Aimbot {
    std::optional<std::shared_ptr<PlayerBasicInterface>> aimbotTarget = std::nullopt;

public:
    static Aimbot &getInstance();

    void callback();
    
};

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
