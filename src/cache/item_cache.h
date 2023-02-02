#ifndef EVIL_CAT_CACHE_ITEM_CACHE_H
#define EVIL_CAT_CACHE_ITEM_CACHE_H

#include <memory>
#include "registry.h"
#include "cache_body.h"
#include "game/item_interface.h"

class ItemCache : public ItemInterface, public CacheRegistry {
public:
    explicit ItemCache(std::shared_ptr<ItemInterface> instance)
            : instance(std::move(instance)) {}

    glm::vec3 getPosition() override {
        CACHE_BODY(position, positionCached, getPosition);
    }

    std::string getName() override {
        CACHE_BODY(name, nameCached, getName);
    }

    std::string getExtra() override {
        CACHE_BODY(extra, extraCached, getExtra);
    }

    int getLevel() override {
        CACHE_BODY(level, levelCached, getLevel);
    }

    void refresh() override {
        positionCached = false;
        nameCached = false;
        extraCached = false;
        levelCached = false;
    }

private:
    std::shared_ptr<ItemInterface> instance;

    bool positionCached = false;
    glm::vec3 position;

    bool nameCached = false;
    std::string name;

    bool extraCached = false;
    std::string extra;

    bool levelCached = false;
    float level;
};

#endif //EVIL_CAT_CACHE_ITEM_CACHE_H
