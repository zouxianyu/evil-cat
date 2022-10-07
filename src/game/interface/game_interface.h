#ifndef EVIL_CAT_GAME_INTERFACE_GAME_INTERFACE_H
#define EVIL_CAT_GAME_INTERFACE_GAME_INTERFACE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "game/interface/player_interface.h"

class GameInterface {
public:
    virtual std::shared_ptr<PlayerInterface> getLocalPlayer() = 0;

    virtual std::vector<std::shared_ptr<PlayerInterface>> getPlayers() = 0;

    virtual glm::mat4 getVPMatrix() = 0;

    virtual glm::vec2 getWindowSize() = 0;

    virtual glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) = 0;

    virtual glm::vec3 orientationToViewAngle(glm::vec3 orientation) = 0;

    virtual float getDistance(std::shared_ptr<PlayerInterface> player) = 0;

    virtual ~GameInterface() = default;
};

#endif //EVIL_CAT_GAME_INTERFACE_GAME_INTERFACE_H
