#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H

#include <memory>
#include <functional>
#include <utility>

struct InitConfig {
    using GuiCallback = std::pair<std::function<bool()>, std::function<void()>>;
    std::vector<GuiCallback> guiCallbacks;

    using FastLoopCallback = std::function<bool()>;
    std::vector<FastLoopCallback> fastLoopCallbacks;

    InitConfig(
        std::vector<GuiCallback> guiCallbacks,
        std::vector<FastLoopCallback> fastLoopCallbacks
    ):  guiCallbacks(std::move(guiCallbacks)),
        fastLoopCallbacks(std::move(fastLoopCallbacks)) {
    };
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H
