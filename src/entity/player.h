#ifndef ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H
#define ASSAULT_CUBE_HACKING_ENTITY_PLAYER_H

#include <string>
#include "type.h"
#include "offset/dynamic_define.h"
#include "offset/offset.h"

class Player {

    union {
        DEFINE_MEMBER_N(Vec3, head, offset::player::head);
        DEFINE_MEMBER_N(Vec3, feet, offset::player::feet);
        DEFINE_MEMBER_N(Vec3, viewAngle, offset::player::viewAngle);
        DEFINE_MEMBER_N(int, health, offset::player::health);
        DEFINE_MEMBER_N(int, armor, offset::player::armor);
        DEFINE_MEMBER_N(Name, name, offset::player::name);
        DEFINE_MEMBER_N(uint8_t, team, offset::player::team);
    };

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
