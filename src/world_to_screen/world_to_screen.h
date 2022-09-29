#ifndef EVIL_CAT_SERVICE_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
#define EVIL_CAT_SERVICE_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H

#include <optional>
#include <functional>
#include <glm/glm.hpp>

class WorldToScreen {
    glm::mat4 vpMatrix; // cached view matrix
    float width;
    float height;

public:
    static WorldToScreen &getInstance();

    bool refresh();

    std::optional<glm::vec2> translate(const glm::vec3 &world);
};

#endif //EVIL_CAT_SERVICE_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
