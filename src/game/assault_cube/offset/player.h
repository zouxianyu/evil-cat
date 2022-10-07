#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include "offset_helper.h"

namespace Offset {

    using Name = std::array<char, 16>;

    namespace Player {
        constexpr auto head = 0x4; // vec3
        constexpr auto feet = 0x34; // vec3
        constexpr auto viewAngle = 0x40; // vec3
        constexpr auto health = 0xF8; // int
        constexpr auto armor = 0xFC; // int
        constexpr auto name = 0x225; // Offset::Name
        constexpr auto team = 0x32C; // uint8_t
    }

}

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_PLAYER_H
