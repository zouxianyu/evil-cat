#ifndef EVIL_CAT_CACHE_GAME_CACHE_H
#define EVIL_CAT_CACHE_GAME_CACHE_H

#include <memory>
#include "registry.h"
#include "cache_body.h"
#include "player_cache.h"
#include "game/game_interface.h"

class GameCache : public GameInterface, public CacheRegistry {
public:
    explicit GameCache(std::unique_ptr<GameInterface> instance)
            : instance(std::move(instance)) {}

    std::shared_ptr<PlayerInterface> getLocalPlayer() override {
        CACHE_BODY_TRANSFORM(localPlayer, localPlayerCached, makeCacheAdapter, getLocalPlayer);
    }

    std::vector<std::shared_ptr<PlayerInterface>> getPlayers() override {
        CACHE_BODY_TRANSFORM(players, playersCached, makeCacheAdapter, getPlayers);
    }

    glm::mat4 getVPMatrix() override {
        CACHE_BODY(vpMatrix, vpMatrixCached, getVPMatrix);
    }

    glm::vec2 getWindowSize() override {
        CACHE_BODY(windowSize, windowSizeCached, getWindowSize);
    }

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) override {
        return instance->viewAngleToOrientation(viewAngle);
    }

    glm::vec3 orientationToViewAngle(glm::vec3 orientation) override {
        return instance->orientationToViewAngle(orientation);
    }

    float getDistance(std::shared_ptr<PlayerInterface> player) override {
        return instance->getDistance(player);
    }

    void refresh() override {
        localPlayerCached = false;
        playersCached = false;
        vpMatrixCached = false;
        windowSizeCached = false;
    }

private:
    std::unique_ptr<GameInterface> instance;

    bool localPlayerCached{};
    std::shared_ptr<PlayerInterface> localPlayer;

    bool playersCached{};
    std::vector<std::shared_ptr<PlayerInterface>> players;

    bool vpMatrixCached{};
    glm::mat4 vpMatrix;

    bool windowSizeCached{};
    glm::vec2 windowSize;

    std::shared_ptr<PlayerInterface>
    makeCacheAdapter(std::shared_ptr<PlayerInterface> player) {
        return std::make_shared<PlayerCache>(std::move(player));
    }

    std::vector<std::shared_ptr<PlayerInterface>>
    makeCacheAdapter(const std::vector<std::shared_ptr<PlayerInterface>> &players) {
        std::vector<std::shared_ptr<PlayerInterface>> result;
        for (auto &player : players) {
            result.emplace_back(makeCacheAdapter(player));
        }
        return result;
    }
};

#endif //EVIL_CAT_CACHE_GAME_CACHE_H
