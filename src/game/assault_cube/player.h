#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_PLAYER_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_PLAYER_H

#include <string>
#include <glm/glm.hpp>
#include "game/interface/player_basic_interface.h"
#include "interface/player_health_interface.h"
#include "interface/player_armor_interface.h"
#include "module_config.h"

class Player : public PlayerBasicInterface,
               public PlayerHealthInterface,
               public PlayerArmorInterface {

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

    bool operator==(const PlayerBasicInterface &other) const override;
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_PLAYER_H
