#ifndef ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H
#define ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H

#include <optional>
#include <memory>
#include <glm/glm.hpp>
#include "game/interface/player_basic_interface.h"

namespace Settings::Aimbot {
    extern bool on;
    extern float maxAngle;
    enum class Policy : int {
        minAngle
    };
    extern Policy policy;
}

class Aimbot {
    std::optional<std::shared_ptr<PlayerBasicInterface>> aimbotTarget = std::nullopt;

    void aimHead(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::shared_ptr<PlayerBasicInterface> targetPlayer
    );

public:
    static Aimbot &getInstance();

    void callback();
    
};

#endif //ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H
