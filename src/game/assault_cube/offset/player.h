#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include "offset_helper.h"

namespace Offset {

    using Name = std::array<char, 16>;

    struct Player {
        union {
            DEFINE_MEMBER_N(glm::vec3, head, 0x4);
            DEFINE_MEMBER_N(glm::vec3, feet, 0x34);
            DEFINE_MEMBER_N(glm::vec3, viewAngle, 0x40);
            DEFINE_MEMBER_N(int, health, 0xF8);
            DEFINE_MEMBER_N(int, armor, 0xFC);
            DEFINE_MEMBER_N(Name, name, 0x225);
            DEFINE_MEMBER_N(uint8_t, team, 0x32C);
        };
    };

}

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H
