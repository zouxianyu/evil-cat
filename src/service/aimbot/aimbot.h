#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H

#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "service/service_interface.h"
#include "game/player_interface.h"

using Trigger = std::function<bool()>;

using TargetPicker = std::function<std::shared_ptr<PlayerInterface>(
        std::shared_ptr<PlayerInterface>,
        const std::vector<std::shared_ptr<PlayerInterface>>&
)>;

using Aimer = std::function<void(
        std::shared_ptr<PlayerInterface>,
        std::shared_ptr<PlayerInterface>
)>;

struct Strategy {
    Trigger trigger;
    TargetPicker targetPicker;
    Aimer aimer;
};

namespace Settings::Aimbot {
    extern bool on;
    extern float maxAngle;
    extern float moveRatio;
    extern Strategy strategy;
    extern bool useBoneAimer;
    extern Bone bone;
    extern float nonBoneAimerRelativeHeight;
    extern bool ignoreInvisiblePlayer;
    extern float dt;
    extern float gravity;
    extern bool showPredictAimPosition;
}

class Aimbot : public ServiceInterface {
public:
    std::string getName() override;

    void menuCallback() override;

    void serviceCallback() override;

private:
    std::shared_ptr<PlayerInterface> aimbotTarget = nullptr;

};

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_H
