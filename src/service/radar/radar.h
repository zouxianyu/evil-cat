#ifndef EVIL_CAT_SERVICE_RADAR_RADAR_H
#define EVIL_CAT_SERVICE_RADAR_RADAR_H

#include <imgui.h>
#include <glm/glm.hpp>
#include "service/service_interface.h"

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

class Radar : public ServiceInterface {
public:
    void callback() override;

};

#endif //EVIL_CAT_SERVICE_RADAR_RADAR_H
