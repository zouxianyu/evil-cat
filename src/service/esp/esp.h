#ifndef ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H
#define ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H

#include <vector>
#include "entity/player.h"
#include "type.h"

class Esp {
    void showEspBox3D(Player& localPlayer, std::vector<Player>& players);
    void showEspBox2D(Player& localPlayer, std::vector<Player>& players);
    void showHealthAndArmor(std::vector<Player> &players);

public:
    static Esp& getInstance();
    void renderCallback();
    Vec3 viewAngleToOrientation(Vec3 viewAngle);
    Vec3 orientationToViewAngle(Vec3 orientation);

};

#endif //ASSAULT_CUBE_HACKING_SERVICE_ESP_ESP_H
