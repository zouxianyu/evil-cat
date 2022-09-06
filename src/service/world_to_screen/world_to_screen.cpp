#include <memory>
#include <imgui.h>
#include "game.h"
#include "mem/process_memory_accessor.h"
#include "world_to_screen.h"

WorldToScreen &WorldToScreen::getInstance() {
    static WorldToScreen instance;
    return instance;
}

bool WorldToScreen::refresh() {

    // refresh view projection matrix
    std::shared_ptr<Mat4> mat = Game::getInstance().getVPMatrix();
    vpMatrix = {
            mat->m[0][0], mat->m[0][1], mat->m[0][2], mat->m[0][3],
            mat->m[1][0], mat->m[1][1], mat->m[1][2], mat->m[1][3],
            mat->m[2][0], mat->m[2][1], mat->m[2][2], mat->m[2][3],
            mat->m[3][0], mat->m[3][1], mat->m[3][2], mat->m[3][3]
    };

    // refresh window size
    Vec2 windowSize = Game::getInstance().getWindowSize();
    width = windowSize.x;
    height = windowSize.y;

    return true;
}

std::optional<Vec2> WorldToScreen::translate(const Vec3 &world) {

    // calculate clip coordinates
    glm::vec4 clip = vpMatrix * glm::vec4{world.x, world.y, world.z, 1.0f};
    if (clip.w < 0.1f) {
        return std::nullopt;
    }

    // calculate NDC coordinates
    glm::vec4 NDC = clip / clip.w;
    Vec2 screen{(NDC.x + 1.0f) * 0.5f * width, (-NDC.y + 1.0f) * 0.5f * height};
    if (screen.x < -width || screen.x >= 2 * width || screen.y < -height || screen.y >= 2 * height) {
        return std::nullopt;
    }
    return screen;
}
