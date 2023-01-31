#ifndef EVIL_CAT_GAME_PUBG_NAME_HELPER_H
#define EVIL_CAT_GAME_PUBG_NAME_HELPER_H

#include <string>
#include "structure.h"
#include "game/player_interface.h"

namespace PUBG::Helper {
    bool isAIPlayer(const std::string &objectName);

    bool isRealPlayer(const std::string &objectName);

    bool isPlayer(const std::string &objectName);

    std::string toString(FString str);

    uint64_t getPlayerController();

    CameraInfo getCameraInfo();

    void setCameraRotation(glm::vec3 rotation);

    int getBoneIndex(Bone bone);
}

namespace PUBG {
    using namespace PUBG::Helper;
}

#endif //EVIL_CAT_GAME_PUBG_NAME_HELPER_H
