#ifndef EVIL_CAT_GAME_CSGO_GAME_H
#define EVIL_CAT_GAME_CSGO_GAME_H

#include "game/game_interface.h"

class Game : public GameInterface {
public:
    std::shared_ptr<PlayerInterface> getLocalPlayer() override;

    std::vector<std::shared_ptr<PlayerInterface>> getPlayers() override;

    glm::mat4 getVPMatrix() override;

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) override;

    glm::vec3 orientationToViewAngle(glm::vec3 orientation) override;
};

#endif //EVIL_CAT_GAME_CSGO_GAME_H
