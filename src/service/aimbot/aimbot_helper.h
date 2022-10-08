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

    extern Strategy rightButtonPrecise;
    extern Strategy rightButtonSmooth;
    extern Strategy autoFollowPrecise;
    extern Strategy autoFollowSmooth;

    bool leftKeyTrigger();

    bool rightKeyTrigger();

    std::optional<std::shared_ptr<PlayerInterface>> minAnglePicker(
            const std::shared_ptr<PlayerInterface>& localPlayer,
            const std::vector<std::shared_ptr<PlayerInterface>> &players
    );

    class SpeedChangeableAimer {
        const float ratio;
    public:
        explicit SpeedChangeableAimer(float ratio) : ratio(ratio) {};
        void operator()(
                const std::shared_ptr<PlayerInterface>& localPlayer,
                const std::shared_ptr<PlayerInterface>& targetPlayer
        );
    };
}
#endif //EVIL_CAT_SERVICE_AIMBOT_AIMBOT_HELPER_H
