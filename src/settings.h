#ifndef ASSAULT_CUBE_HACKING_SETTINGS_H
#define ASSAULT_CUBE_HACKING_SETTINGS_H

#include <optional>
#include <mutex>
#include <glm/glm.hpp>
#include <imgui.h>

class Settings {
public:
    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    // must hold the mutex before using all the fields below
    std::mutex mutex;

    bool showEsp = true;
    bool showEspBox2D = true;
    bool showHealthAndArmor = true;
    bool showName = true;

    bool aimbot = true;
    float aimbotMaxAngle = glm::radians(10.f);


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
