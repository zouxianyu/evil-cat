#ifndef EVIL_CAT_GAME_CSGO_GAME_H
#define EVIL_CAT_GAME_CSGO_GAME_H

#include "game/game_interface.h"

class Game : public GameInterface {
public:
    EntityContainer getEntities() override;

    glm::mat4 getVPMatrix() override;

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) override;

    glm::vec3 orientationToViewAngle(glm::vec3 orientation) override;

private:
    void getLocalPlayer(EntityContainer &container);

    void getPlayers(EntityContainer &container);

};

#endif //EVIL_CAT_GAME_CSGO_GAME_H
