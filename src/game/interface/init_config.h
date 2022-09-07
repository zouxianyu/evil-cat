#ifndef EVIL_CAT_GAME_INTERFACE_INIT_CONFIG_H
#define EVIL_CAT_GAME_INTERFACE_INIT_CONFIG_H

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

#endif //EVIL_CAT_GAME_INTERFACE_INIT_CONFIG_H
