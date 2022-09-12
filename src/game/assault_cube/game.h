#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H

#include <vector>
#include <memory>
#include "game/interface/player_basic_interface.h"

class Game {
public:
    static Game &getInstance();

    std::shared_ptr<PlayerBasicInterface> getLocalPlayer();

    std::vector<std::shared_ptr<PlayerBasicInterface>> getPlayers();

    std::shared_ptr<glm::mat4> getVPMatrix();

    glm::vec2 getWindowSize();

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle);

    glm::vec3 orientationToViewAngle(glm::vec3 orientation);
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H
