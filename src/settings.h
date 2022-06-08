#ifndef ASSAULT_CUBE_HACKING_SETTINGS_H
#define ASSAULT_CUBE_HACKING_SETTINGS_H

#include <optional>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include "entity/player.h"

class Settings {
public:
    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    bool showEsp = true;
    bool showEspBox2D = true;
    bool showHealthAndArmor = true;
    bool showName = true;

    bool aimbot = true;
    float aimbotMaxAngle = glm::radians(10.f);
    std::optional<Player> aimbotTarget = std::nullopt;

    bool exit = false;

    ImColor textColor = ImColor(255, 255, 255);
    ImColor friendlyColor = ImColor(0, 255, 0);
    ImColor enemyColor = ImColor(255, 0, 0);
    ImColor viewLineColor = ImColor(255, 255, 255);
    ImColor healthColor = ImColor(0, 255, 0);
    ImColor armorColor = ImColor(0, 0, 255);
    ImColor backgroundColor = ImColor(0, 0, 0);
};

#endif //ASSAULT_CUBE_HACKING_SETTINGS_H
