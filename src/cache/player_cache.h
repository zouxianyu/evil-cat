#ifndef EVIL_CAT_CACHE_PLAYER_CACHE_H
#define EVIL_CAT_CACHE_PLAYER_CACHE_H

#include <memory>
#include "registry.h"
#include "cache_body.h"
#include "game/player_interface.h"

class PlayerCache : public PlayerInterface, public CacheRegistry {
public:
    explicit PlayerCache(std::shared_ptr<PlayerInterface> instance)
            : instance(std::move(instance)) {}

    glm::vec3 getPosition() override {
        CACHE_BODY(position, positionCached, getPosition);
    }

    float getHeight() override {
        CACHE_BODY(height, heightCached, getHeight);
    }

    glm::vec3 getCameraPosition() override {
        CACHE_BODY(cameraPosition, cameraPositionCached, getCameraPosition);
    }

    glm::vec3 getViewAngle() override {
        CACHE_BODY(viewAngle, viewAngleCached, getViewAngle);
    }

    void setViewAngle(glm::vec3 angle) override {
        instance->setViewAngle(angle);
    }

    std::string getName() override {
        CACHE_BODY(name, nameCached, getName);
    }

    int getTeamId() override {
        CACHE_BODY(teamId, teamIdCached, getTeamId);
    }

    float getHealth() override {
        CACHE_BODY(health, healthCached, getHealth);
    }

    void setHealth(float health) override {
        instance->setHealth(health);
    }

    float getArmor() override {
        CACHE_BODY(armor, armorCached, getArmor);
    }

    void setArmor(float armor) override {
        instance->setArmor(armor);
    }

    BoneArray getBonePositions() override {
        CACHE_BODY(bonePositions, bonePositionsCached, getBonePositions);
    }

    Weapon getWeapon() override {
        CACHE_BODY(weapon, weaponCached, getWeapon);
    }

    bool isVisible() override {
        CACHE_BODY(visible, visibleCached, isVisible);
    }

    bool operator==(const PlayerInterface &other) const override {
        return *instance == *dynamic_cast<const PlayerCache &>(other).instance;
    }

    void refresh() override {
        positionCached = false;
        heightCached = false;
        cameraPositionCached = false;
        viewAngleCached = false;
        nameCached = false;
        teamIdCached = false;
        healthCached = false;
        armorCached = false;
        bonePositionsCached = false;
    }

private:
    std::shared_ptr<PlayerInterface> instance;

    bool positionCached{};
    glm::vec3 position;

    bool heightCached{};
    float height;

    bool cameraPositionCached{};
    glm::vec3 cameraPosition;

    bool viewAngleCached{};
    glm::vec3 viewAngle;

    bool nameCached{};
    std::string name;

    bool teamIdCached{};
    int teamId;

    bool healthCached{};
    float health;

    bool armorCached{};
    float armor;

    bool bonePositionsCached{};
    BoneArray bonePositions;

    bool weaponCached{};
    Weapon weapon;

    bool visibleCached{};
    bool visible;
};

#endif //EVIL_CAT_CACHE_PLAYER_CACHE_H
