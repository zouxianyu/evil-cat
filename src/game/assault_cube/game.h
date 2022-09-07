#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H

#include <vector>
#include <memory>
#include "game/interface/player_basic_interface.h"
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

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_GAME_H
