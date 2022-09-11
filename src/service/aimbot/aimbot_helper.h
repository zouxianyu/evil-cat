#ifndef EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H
#define EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H

#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include "game/interface/player_basic_interface.h"

namespace AimbotHelper {

    typedef std::function<std::optional<std::shared_ptr<PlayerBasicInterface>>(
            std::shared_ptr<PlayerBasicInterface>,
    const std::vector<std::shared_ptr<PlayerBasicInterface>>&
    )> TargetPicker;

    typedef std::function<bool()> Trigger;

    typedef std::function<void(
            std::shared_ptr<PlayerBasicInterface>,
            std::shared_ptr<PlayerBasicInterface>
    )> Aimer;

    struct Strategy {
        Trigger trigger;
        TargetPicker targetPicker;
        Aimer aimer;
    };

    extern Strategy rightButtonPrecise;
    extern Strategy rightButtonSmooth;
    extern Strategy autoFollowPrecise;
    extern Strategy autoFollowSmooth;

    bool leftKeyTrigger();

    bool rightKeyTrigger();

    std::optional<std::shared_ptr<PlayerBasicInterface>> minAnglePicker(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            const std::vector<std::shared_ptr<PlayerBasicInterface>> &players
    );

    template<int divisor>
    void speedChangeableAimer(
            std::shared_ptr<PlayerBasicInterface> localPlayer,
            std::shared_ptr<PlayerBasicInterface> targetPlayer
    );
}
#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H
