#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_STRATEGY_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_STRATEGY_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include "game/player_interface.h"

namespace AimbotStrategy {

    bool leftKeyTrigger();

    bool rightKeyTrigger();

    bool leftOrRightKeyTrigger();

    std::shared_ptr<PlayerInterface> minAnglePicker(
            std::shared_ptr<PlayerInterface> localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>> &players
    );

    void speedChangeableAimer(
            std::shared_ptr<PlayerInterface> localPlayer,
            std::shared_ptr<PlayerInterface> targetPlayer
    );
}

#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_STRATEGY_H
