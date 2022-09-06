#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_GAME_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_GAME_H

#include <vector>
#include <memory>
#include "type.h"

class Game {
public:
    static Game &getInstance();

    std::shared_ptr<PlayerBasicInterface> getLocalPlayer();

    std::vector<std::shared_ptr<PlayerBasicInterface>> getPlayers();

    std::shared_ptr<Mat4> getVPMatrix();

    Vec2 getWindowSize();

    Vec3 viewAngleToOrientation(Vec3 viewAngle);

    Vec3 orientationToViewAngle(Vec3 orientation);
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_GAME_H
