#include <memory>
#include <glm/glm.hpp>
#include "module.h"
#include "world_to_screen.h"

std::optional<glm::vec2> WorldToScreen::translate(const glm::vec3 &world) {

    glm::mat4 vpMatrix = Module::game->getVPMatrix();
    glm::vec2 windowSize = Module::game->getWindowSize();
    const float &width = windowSize.x;
    const float &height = windowSize.y;

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
