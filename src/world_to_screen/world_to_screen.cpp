#include <memory>
#include <imgui.h>
#include <glm/glm.hpp>
#include "game.h"
#include "mem/process_memory_accessor.h"
#include "world_to_screen.h"

WorldToScreen &WorldToScreen::getInstance() {
    static WorldToScreen instance;
    return instance;
}

bool WorldToScreen::refresh() {

    // refresh view projection matrix
    vpMatrix = Game::getInstance().getVPMatrix();

    // refresh window size
    glm::vec2 windowSize = Game::getInstance().getWindowSize();
    width = windowSize.x;
    height = windowSize.y;

    return true;
}

std::optional<glm::vec2> WorldToScreen::translate(const glm::vec3 &world) {

    // calculate clip coordinates
    glm::vec4 clip = vpMatrix * glm::vec4{world.x, world.y, world.z, 1.0f};
    if (clip.w < 0.1f) {
        return std::nullopt;
    }

    // calculate NDC coordinates
    glm::vec4 NDC = clip / clip.w;
    glm::vec2 screen{
            (NDC.x + 1.0f) * 0.5f * width,
            (-NDC.y + 1.0f) * 0.5f * height
    };
    if (screen.x < -width ||
        screen.x >= 2 * width ||
        screen.y < -height ||
        screen.y >= 2 * height
    ) {
        return std::nullopt;
    }
    return screen;
}
