#ifndef ASSAULT_CUBE_HACKING_ENTITY_ENTITY_MANAGER_H
#define ASSAULT_CUBE_HACKING_ENTITY_ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "player.h"

class EntityManager {

public:
    EntityManager(const EntityManager &) = delete;

    EntityManager &operator=(const EntityManager &) = delete;

    EntityManager(EntityManager &&) = delete;

    EntityManager &operator=(EntityManager &&) = delete;

    EntityManager();

    static EntityManager &getInstance();

    Player getLocalPlayer();

    std::vector<Player> getPlayers();

    glm::mat4 getViewProjectionMatrix();
};

#endif //ASSAULT_CUBE_HACKING_ENTITY_ENTITY_MANAGER_H
