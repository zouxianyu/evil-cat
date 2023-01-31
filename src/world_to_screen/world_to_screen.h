#ifndef EVIL_CAT_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
#define EVIL_CAT_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H

#include <optional>
#include <functional>
#include <glm/glm.hpp>

namespace WorldToScreen {
    std::optional<glm::vec2> translate(const glm::vec3 &world);
}

#endif //EVIL_CAT_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
