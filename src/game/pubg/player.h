#ifndef EVIL_CAT_GAME_PUBG_PLAYER_H
#define EVIL_CAT_GAME_PUBG_PLAYER_H

#include "game/player_interface.h"

class Player : public PlayerInterface {
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

    BoneArray getBonePositions() override;

    Weapon getWeapon() override;

    bool isVisible() override;

    bool operator==(const PlayerInterface &other) const override;

protected:
    gameptr_t _this;
};

class LocalPlayer : public Player {
public:
    using Player::Player;

    glm::vec3 getCameraPosition() override;

    glm::vec3 getViewAngle() override;

    void setViewAngle(glm::vec3 angle) override;
};

#endif //EVIL_CAT_GAME_PUBG_PLAYER_H
