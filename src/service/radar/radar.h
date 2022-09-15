#ifndef EVIL_CAT_SERVICE_RADAR_RADAR_H
#define EVIL_CAT_SERVICE_RADAR_RADAR_H

#include <imgui.h>
#include <glm/glm.hpp>

namespace Settings::Radar {
    extern bool on;
    extern glm::vec2 normalizedCenter;
    extern float radius;
    extern float scale;
    extern bool rotate;
    extern float playerRadius;
    extern float viewLineScale;
    extern ImColor backgroundColor;
    extern ImColor borderColor;
    extern ImColor teammateColor;
    extern ImColor enemyColor;
}

class Radar {

public:
    static Radar &getInstance();

    void callback();
};

#endif //EVIL_CAT_SERVICE_RADAR_RADAR_H
