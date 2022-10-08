#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "service/service_interface.h"
#include "game/player_interface.h"

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
    extern Bone bone;
}

class Aimbot : public ServiceInterface {
    std::optional<std::shared_ptr<PlayerInterface>> aimbotTarget = std::nullopt;

public:
    void callback() override;

};

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
