#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_OFFSET_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_OFFSET_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>

// v1.2.0.2
namespace Offset {

    using Name = std::array<char, 16>;

    constexpr auto localPlayer = 0x10F4F4;
    constexpr auto playerList = 0x10F4F8;
    constexpr auto playerCount = 0x10F500;
    constexpr auto vpMatrix = 0x101AE8;

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

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_OFFSET_OFFSET_H
