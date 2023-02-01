#ifndef EVIL_CAT_GAME_BONE_H
#define EVIL_CAT_GAME_BONE_H

#include <array>
#include <glm/glm.hpp>
#include "magic_enum.h"

// bone types
enum class Bone : int {
    head,
    neck,
    leftShoulder,
    rightShoulder,
    leftElbow,
    rightElbow,
    leftHand,
    rightHand,
    spine,
    hip,
    leftHip,
    rightHip,
    leftKnee,
    rightKnee,
    leftFoot,
    rightFoot,
};

using BoneArray = std::array<glm::vec3, magic_enum::enum_count<Bone>()>;

#endif //EVIL_CAT_GAME_BONE_H
