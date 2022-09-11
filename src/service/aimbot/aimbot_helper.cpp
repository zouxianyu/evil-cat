#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <windows.h>
#include "game.h"
#include "aimbot.h"
#include "game/interface/player_basic_interface.h"
#include "type.h"
#include "aimbot_helper.h"

static float getDeltaAngle(Vec3 o1, Vec3 o2) {
    glm::vec3 v1{o1.x, o1.y, o1.z};
    glm::vec3 v2{o2.x, o2.y, o2.z};
    return glm::angle(glm::normalize(v1), glm::normalize(v2));
}

namespace AimbotHelper {
    Strategy rightButtonPrecise = {
            rightKeyTrigger,
            minAnglePicker,
            speedChangeableAimer<1>
    };
    Strategy rightButtonSmooth = {
            rightKeyTrigger,
            minAnglePicker,
            speedChangeableAimer<3>
    };
    Strategy autoFollowPrecise = {
            leftKeyTrigger,
            minAnglePicker,
            speedChangeableAimer<1>
    };
    Strategy autoFollowSmooth = {
            leftKeyTrigger,
            minAnglePicker,
            speedChangeableAimer<3>
    };
}

bool AimbotHelper::leftKeyTrigger() {
    return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
}

bool AimbotHelper::rightKeyTrigger() {
    return GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

std::optional<std::shared_ptr<PlayerBasicInterface>> AimbotHelper::minAnglePicker(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        const std::vector<std::shared_ptr<PlayerBasicInterface>> &players
) {

    // get local player orientation
    Vec3 localPlayerViewAngle = localPlayer->getViewAngle();
    Vec3 localPlayerOrientation = Game::getInstance().viewAngleToOrientation(localPlayerViewAngle);

    // find the best enemy
    // maybe there's no valid enemy, so we use "optional" container
    std::optional<std::shared_ptr<PlayerBasicInterface>> bestEnemy = std::nullopt;
    double minAngle = glm::pi<double>();

    for (auto &player: players) {

        // if the player is not a valid enemy (death or teammates)
        if (*player == *localPlayer ||
            player->getHealth() <= 0 ||
            player->getTeamId() == localPlayer->getTeamId()) {
            continue;
        }

        // get target orientation
        Vec3 targetOrientation = player->getCameraPosition() - localPlayer->getCameraPosition();

        // calculate delta view angle
        double deltaAngle = getDeltaAngle(localPlayerOrientation, targetOrientation);

        // make sure the enemy's delta angle is less than the threshold
        if (deltaAngle >= Settings::Aimbot::maxAngle) {
            continue;
        }

        if (deltaAngle < minAngle) {
            // a better enemy is found
            minAngle = deltaAngle;
            bestEnemy = player;
        }
    }

    // return the best enemy (maybe null)
    return bestEnemy;
}

template<int divisor>
void AimbotHelper::speedChangeableAimer(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::shared_ptr<PlayerBasicInterface> targetPlayer
) {
    Vec3 targetPos = targetPlayer->getCameraPosition();
    Vec3 localPos = localPlayer->getCameraPosition();
    Vec3 targetOrientation = targetPos - localPos;
    Vec3 currentOrientation = Game::getInstance().viewAngleToOrientation(
            localPlayer->getViewAngle()
    );
    glm::vec3 vCurrent = glm::normalize(glm::vec3{
        currentOrientation.x, currentOrientation.y, currentOrientation.z
    });
    glm::vec3 vTarget = glm::normalize(glm::vec3{
        targetOrientation.x, targetOrientation.y, targetOrientation.z
    });
    glm::vec3 vCross = glm::cross(vCurrent, vTarget);
    float angle = glm::angle(vCurrent, vTarget) / divisor;
    glm::vec3 vAim = glm::rotate(vCurrent, angle, vCross);
    Vec3 aimOrientation = {vAim.x, vAim.y, vAim.z};
    Vec3 aimViewAngle = Game::getInstance().orientationToViewAngle(aimOrientation);
    localPlayer->setViewAngle(aimViewAngle);
}
