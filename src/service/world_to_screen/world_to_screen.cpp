#include <imgui/imgui.h>
#include "entity/entity_manager.h"
#include "offset/offset.h"
#include "mem/process_memory_accessor.h"

#include "world_to_screen.h"
#include "game_matrix_world_to_screen.h"
#include "self_matrix_world_to_screen.h"

WorldToScreen &WorldToScreen::getInstance() {
#if (CONF_USE_GAME_VIEW_MATRIX == 1)
    return GameMatrixWorldToScreen::getInstance();
#else
    return SelfMatrixWorldToScreen::getInstance();
#endif
}

bool WorldToScreen::refresh() {
    vpMatrix = getViewProjectionMatrix();
//    width = ImGui::GetWindowWidth();
//    height = ImGui::GetWindowHeight();
    width = 1024;
    height = 768;
    return true;
}

bool WorldToScreen::translate(const Vec3 &world, Vec2 &screen) {

    glm::vec4 clip = vpMatrix * glm::vec4{world.x, world.y, world.z, 1.0f};
    if (clip.w < 0.1f) {
        return false;
    }
    glm::vec4 NDC = clip / clip.w;
    Vec2 screenTry;
    screenTry.x = (NDC.x + 1.0f) * 0.5f * width;
    screenTry.y = (-NDC.y + 1.0f) * 0.5f * height;
    if (screenTry.x < -width || screenTry.x >= 2*width || screenTry.y < -height || screenTry.y >= 2*height) {
        return false;
    }
    screen = screenTry;
    return true;
}
