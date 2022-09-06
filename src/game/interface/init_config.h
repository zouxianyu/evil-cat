#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H

#include <memory>
#include <functional>
#include <utility>

struct InitConfig {
    std::vector<std::function<void()>> guiCallbacks;
    std::vector<std::function<bool()>> fastLoopCallbacks;

    InitConfig(
        std::vector<std::function<void()>> guiCallbacks,
        std::vector<std::function<bool()>> fastLoopCallbacks
    ):  guiCallbacks(std::move(guiCallbacks)),
        fastLoopCallbacks(std::move(fastLoopCallbacks)) {
    };
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_INIT_INTERFACE_H
