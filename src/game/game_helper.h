#ifndef EVIL_CAT_GAME_GAME_HELPER_H
#define EVIL_CAT_GAME_GAME_HELPER_H

#include <vector>
#include <memory>
#include <type_traits>
#include "game.h"
#include "player.h"

namespace GameHelper {
    template<typename T>
    std::vector<std::shared_ptr<T>>
    getPlayers() {
        static_assert(std::is_base_of_v<T, Player>, "T must be a player interface");
        auto players = Game::getInstance().getPlayers();
        std::vector<std::shared_ptr<T>> castPlayers;
        for (auto &player : players) {
            castPlayers.emplace_back(std::move(player));
        }
        return castPlayers;
    }
}

#endif //EVIL_CAT_GAME_GAME_HELPER_H
