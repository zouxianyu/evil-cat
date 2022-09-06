#ifndef ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
#define ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H

#include <optional>
#include <functional>
#include <glm/glm.hpp>
#include "type.h"

class WorldToScreen {
    glm::mat4 vpMatrix; // cached view matrix
    float width;
    float height;

public:
    static WorldToScreen &getInstance();

    bool refresh();

    std::optional<Vec2> translate(const Vec3 &world);
};

#endif //ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
