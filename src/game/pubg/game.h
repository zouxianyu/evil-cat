#ifndef EVIL_CAT_GAME_PUBG_GAME_H
#define EVIL_CAT_GAME_PUBG_GAME_H

#include "structure.h"
#include "game/game_interface.h"

class Game : public GameInterface {
public:
    EntityContainer getEntities() override;

    glm::mat4 getVPMatrix() override;

    glm::vec3 viewAngleToOrientation(glm::vec3 viewAngle) override;

    glm::vec3 orientationToViewAngle(glm::vec3 orientation) override;

private:
    void getLocalPlayer(EntityContainer &container);

    void getPlayersAndItems(EntityContainer &container);

    glm::mat4 getInverseRotationMatrix(FRotator rotation);

    glm::mat4 getViewMatrix(const CameraInfo &camera);

    glm::mat4 getProjectionMatrix(const CameraInfo &camera);
};

#endif //EVIL_CAT_GAME_PUBG_GAME_H
