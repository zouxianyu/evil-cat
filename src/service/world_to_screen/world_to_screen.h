#ifndef ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
#define ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H

#include <functional>
#include <glm/glm.hpp>
#include "type.h"

class WorldToScreen {
    glm::mat4 vpMatrix{}; // cached view matrix
    float width{};
    float height{};

    virtual glm::mat4 getViewProjectionMatrix() = 0;

public:
    static WorldToScreen &getInstance();

    bool refresh();

    bool translate(const Vec3 &world, Vec2 &screen);
};

#endif //ASSAULT_CUBE_HACKING_WORLD_TO_SCREEN_WORLD_TO_SCREEN_H
