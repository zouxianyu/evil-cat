#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include "game/player_interface.h"

namespace AimbotHelper {

    typedef std::function<std::optional<std::shared_ptr<PlayerInterface>>(
            std::shared_ptr<PlayerInterface>,
            const std::vector<std::shared_ptr<PlayerInterface>>&
    )> TargetPicker;

    typedef std::function<bool()> Trigger;

    typedef std::function<void(
            std::shared_ptr<PlayerInterface>,
            std::shared_ptr<PlayerInterface>
    )> Aimer;

    struct Strategy {
        Trigger trigger;
        TargetPicker targetPicker;
        Aimer aimer;
    };

    extern Strategy triggerOnRightButton;
    extern Strategy triggerOnLeftButton;

    bool leftKeyTrigger();

    bool rightKeyTrigger();

    std::optional<std::shared_ptr<PlayerInterface>> minAnglePicker(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>> &players
    );

    void speedChangeableAimer(
            const std::shared_ptr<PlayerInterface> &localPlayer,
            const std::shared_ptr<PlayerInterface> &targetPlayer
    );
}
#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H
