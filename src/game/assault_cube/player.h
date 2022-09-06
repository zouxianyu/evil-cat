#ifndef ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H
#define ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H

#include <string>
#include "type.h"
#include "game/interface/player_basic_interface.h"
#include "interface/player_health_interface.h"
#include "interface/player_armor_interface.h"

class Player : public PlayerBasicInterface,
               public PlayerHealthInterface,
               public PlayerArmorInterface {

    void *_this;

public:
    explicit Player(void *address);

    Vec3 getPosition() override;

    float getHeight() override;

    Vec3 getCameraPosition() override;

    Vec3 getViewAngle() override;

    void setViewAngle(Vec3 angle) override;

    std::string getName() override;

    int getTeamId() override;

    float getHealth() override;

    void setHealth(float health) override;

    float getArmor() override;

    void setArmor(float armor) override;

    bool operator==(const PlayerBasicInterface &other) const override;
};

#endif //ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H
