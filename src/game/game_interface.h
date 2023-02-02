#ifndef EVIL_CAT_GAME_GAME_INTERFACE_H
#define EVIL_CAT_GAME_GAME_INTERFACE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "entity_container.h"

class GameInterface {
public:
    virtual EntityContainer getEntities() = 0;

    virtual glm::mat4 getVPMatrix() = 0;

    virtual glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) = 0;

    virtual glm::vec3 orientationToViewAngle(glm::vec3 orientation) = 0;

    virtual ~GameInterface() = default;
};

#endif //EVIL_CAT_GAME_GAME_INTERFACE_H
