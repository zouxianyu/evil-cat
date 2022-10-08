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
    extern float moveRatio;
    enum class Strategy : int {
        triggerOnRightButton,
        triggerOnLeftButton,
    };
    extern Strategy strategy;
    extern bool useBoneAimer;
    extern Bone bone;
    extern float nonBoneAimerRelativeHeight;
}

class Aimbot : public ServiceInterface {
    std::optional<std::shared_ptr<PlayerInterface>> optAimbotTarget = std::nullopt;

public:
    void callback() override;

};

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
