#include "entity/entity_manager.h"
#include "game_matrix_world_to_screen.h"

glm::mat4 GameMatrixWorldToScreen::getViewProjectionMatrix() {
    return EntityManager::getInstance().getViewProjectionMatrix();
}

GameMatrixWorldToScreen &GameMatrixWorldToScreen::getInstance() {
    static GameMatrixWorldToScreen instance;
    return instance;
}
