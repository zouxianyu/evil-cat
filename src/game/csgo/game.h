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

    void getPlayersAndItems(EntityContainer &container);

    std::vector<uint32_t> getEntityList();

    uint32_t getClassID(uint32_t entity);

    std::string getItemName(uint32_t id);

    bool isDormant(uint32_t entity);
};

#endif //EVIL_CAT_GAME_CSGO_GAME_H
