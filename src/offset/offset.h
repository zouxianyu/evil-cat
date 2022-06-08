#ifndef ASSAULT_CUBE_HACKING_OFFSET_OFFSET_H
#define ASSAULT_CUBE_HACKING_OFFSET_OFFSET_H

#include <cstdint>

namespace offset {
    constexpr uintptr_t localPlayer = 0x10F4F4;
    constexpr uintptr_t playerList = 0x10F4F8;
    constexpr uintptr_t playerCount = 0x10F500;
    constexpr uintptr_t vpMatrix = 0x101AE8;

    namespace player {
        constexpr uintptr_t head = 0x4;
        constexpr uintptr_t feet = 0x34;
        constexpr uintptr_t viewAngle = 0x40;
        constexpr uintptr_t health = 0xF8;
        constexpr uintptr_t armor = 0xFC;
        constexpr uintptr_t name = 0x225;
        constexpr uintptr_t team = 0x32C;
    }
}


#endif //ASSAULT_CUBE_HACKING_OFFSET_OFFSET_H
