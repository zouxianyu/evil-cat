#ifndef ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_GAME_MATRIX_WORLD_TO_SCREEN_H
#define ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_GAME_MATRIX_WORLD_TO_SCREEN_H

#include <glm/glm.hpp>
#include "world_to_screen.h"

class GameMatrixWorldToScreen : public WorldToScreen {

    glm::mat4 getViewProjectionMatrix() override;

public:
    static GameMatrixWorldToScreen &getInstance();
};

#endif //ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_GAME_MATRIX_WORLD_TO_SCREEN_H
