#ifndef EVIL_CAT_GAME_CSGO_PLAYER_H
#define EVIL_CAT_GAME_CSGO_PLAYER_H

#include "game/player_interface.h"

// for bone id
// https://guidedhacking.com/threads/csgo-bone-esp-models-updated.10496/
enum class PlayerType {
    ctm_idf,
    tm_leet,
    unknown
};

class Player : public PlayerInterface {

    glm::vec3 getBoneById(int id);

    PlayerType getPlayerType();

protected:
    gameptr_t _this;

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
