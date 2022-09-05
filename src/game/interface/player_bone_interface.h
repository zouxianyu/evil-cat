#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_PLAYER_BONE_INTERFACE_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_PLAYER_BONE_INTERFACE_H

#include "type.h"

class PlayerBoneInterface {
public:
    enum Bone {
        HEAD,
        NECK,
        CHEST,
        STOMACH,
        LEFT_ARM,
        RIGHT_ARM,
        LEFT_LEG,
        RIGHT_LEG
    };

    virtual Vec3 getBonePosition(Bone bone) = 0;
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_INTERFACE_PLAYER_BONE_INTERFACE_H
