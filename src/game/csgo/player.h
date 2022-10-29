#ifndef EVIL_CAT_GAME_CSGO_PLAYER_H
#define EVIL_CAT_GAME_CSGO_PLAYER_H

#include "game/player_interface.h"

class Player : public PlayerInterface {

    gameptr_t _this;

    glm::vec3 getBoneById(int id);

public:
    explicit Player(gameptr_t address);

    glm::vec3 getPosition() override;

    float getHeight() override;

    glm::vec3 getCameraPosition() override;

    glm::vec3 getViewAngle() override;

    void setViewAngle(glm::vec3 angle) override;

    std::string getName() override;

    int getTeamId() override;

    float getHealth() override;

    void setHealth(float health) override;

    float getArmor() override;

    void setArmor(float armor) override;

    glm::vec3 getBonePosition(Bone boneType) override;

    bool operator==(const PlayerInterface &other) const override;

};

class LocalPlayer : public Player {
public:
    using Player::Player;

    glm::vec3 getViewAngle() override;

    void setViewAngle(glm::vec3 angle) override;
};

#endif //EVIL_CAT_GAME_CSGO_PLAYER_H
