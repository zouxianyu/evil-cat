#ifndef EVIL_CAT_CACHE_GAME_CACHE_H
#define EVIL_CAT_CACHE_GAME_CACHE_H

#include <memory>
#include "registry.h"
#include "cache_body.h"
#include "player_cache.h"
#include "item_cache.h"
#include "game/game_interface.h"

class GameCache : public GameInterface, public CacheRegistry {
public:
    explicit GameCache(std::unique_ptr<GameInterface> instance)
            : instance(std::move(instance)) {}

    EntityContainer getEntities() override {
        CACHE_BODY_TRANSFORM(entities, entitiesCached, makeCacheAdapter, getEntities);
    }

    glm::mat4 getVPMatrix() override {
        CACHE_BODY(vpMatrix, vpMatrixCached, getVPMatrix);
    }

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) override {
        return instance->viewAngleToOrientation(viewAngle);
    }

    glm::vec3 orientationToViewAngle(glm::vec3 orientation) override {
        return instance->orientationToViewAngle(orientation);
    }

    void refresh() override {
        entitiesCached = false;
        vpMatrixCached = false;
    }

private:
    std::unique_ptr<GameInterface> instance;

    bool entitiesCached{};
    EntityContainer entities;

    bool vpMatrixCached{};
    glm::mat4 vpMatrix;

    EntityContainer makeCacheAdapter(const EntityContainer &container) {
        EntityContainer result;
        result.localPlayer = std::make_shared<PlayerCache>(container.localPlayer);
        for (auto &player : container.players) {
            result.players.emplace_back(std::make_shared<PlayerCache>(player));
        }
        for (auto &item : container.items) {
            result.items.emplace_back(std::make_shared<ItemCache>(item));
        }
        return result;
    }
};

#endif //EVIL_CAT_CACHE_GAME_CACHE_H
