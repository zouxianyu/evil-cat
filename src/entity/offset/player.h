#ifndef ASSAULT_CUBE_HACKING_ENTITY_OFFSET_PLAYER_H
#define ASSAULT_CUBE_HACKING_ENTITY_OFFSET_PLAYER_H

#include <cstdint>
#include "type.h"
#include "offset_helper.h"

namespace offset {

    struct Player {
        union {
            DEFINE_MEMBER_N(Vec3, head, 0x4);
            DEFINE_MEMBER_N(Vec3, feet, 0x34);
            DEFINE_MEMBER_N(Vec3, viewAngle, 0x40);
            DEFINE_MEMBER_N(int, health, 0xF8);
            DEFINE_MEMBER_N(int, armor, 0xFC);
            DEFINE_MEMBER_N(Name, name, 0x225);
            DEFINE_MEMBER_N(uint8_t, team, 0x32C);
        };
    };

}

#endif //ASSAULT_CUBE_HACKING_ENTITY_OFFSET_PLAYER_H
