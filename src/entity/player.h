#ifndef ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H
#define ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H

#include <string>
#include "type.h"
#include "entity/offset/offset.h"

class Player {

    void *_this;

public:
    explicit Player (void* address);

    Vec3 getHead();

    Vec3 getFeet();

    Vec3 getViewAngle();

    void setViewAngle(Vec3 angle);

    int getHealth();

    void setHealth(int health);

    int getArmor();

    void setArmor(int armor);

    std::string getName();

    uint8_t getTeam();

};

#endif //ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H
