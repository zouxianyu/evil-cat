#ifndef ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_SELF_MATRIX_WORLD_TO_SCREEN_H
#define ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_SELF_MATRIX_WORLD_TO_SCREEN_H

#include <glm/glm.hpp>
#include "type.h"
#include "world_to_screen.h"

class SelfMatrixWorldToScreen : public WorldToScreen {
    glm::mat4 getViewProjectionMatrix() override;

public:
    static SelfMatrixWorldToScreen &getInstance();

};

#endif //ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_SELF_MATRIX_WORLD_TO_SCREEN_H
