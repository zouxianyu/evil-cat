#include <optional>
#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <windows.h>
#include "aimbot.h"
#include "module.h"
#include "aimbot_strategy.h"

static float getDeltaAngle(glm::vec3 v1, glm::vec3 v2) {
    return glm::angle(glm::normalize(v1), glm::normalize(v2));
}

static glm::vec3 getAimPosition(const std::shared_ptr<PlayerInterface>& player) {
    if (Settings::Aimbot::useBoneAimer) {
        return player->getBonePositions()[static_cast<int>(Settings::Aimbot::bone)];
    } else {
        glm::vec3 position = player->getPosition();
        position.z += player->getHeight() * Settings::Aimbot::nonBoneAimerRelativeHeight;
        return position;
    }
}

bool AimbotStrategy::leftKeyTrigger() {
    return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
}

bool AimbotStrategy::rightKeyTrigger() {
    return GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

bool AimbotStrategy::leftOrRightKeyTrigger() {
    return leftKeyTrigger() || rightKeyTrigger();
}

std::shared_ptr<PlayerInterface> AimbotStrategy::minAnglePicker(
        std::shared_ptr<PlayerInterface> localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>> &players
) {

    // get local player orientation
    glm::vec3 localPlayerViewAngle = localPlayer->getViewAngle();
    glm::vec3 localPlayerOrientation = Module::game->viewAngleToOrientation(
            localPlayerViewAngle
    );

    // find the best enemy
    // maybe there's no valid enemy, so we use "optional" container
    std::shared_ptr<PlayerInterface> bestEnemy = nullptr;
    auto minAngle = glm::pi<double>();

    for (auto &player: players) {

        // if the player is not a valid enemy (death or teammates)
        if (*player == *localPlayer ||
            player->getHealth() <= 0 ||
            player->getTeamId() == localPlayer->getTeamId()) {
            continue;
        }

        // filter by angle
        glm::vec3 targetOrientation = getAimPosition(player) - localPlayer->getCameraPosition();
        double deltaAngle = getDeltaAngle(localPlayerOrientation, targetOrientation);
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

void AimbotStrategy::speedChangeableAimer(
        std::shared_ptr<PlayerInterface> localPlayer,
        std::shared_ptr<PlayerInterface> targetPlayer
) {
    glm::vec3 targetPos = getAimPosition(targetPlayer);
    glm::vec3 localPos = localPlayer->getCameraPosition();
    glm::vec3 vTarget = glm::normalize(targetPos - localPos);
    glm::vec3 vCurrent = glm::normalize(Module::game->viewAngleToOrientation(
            localPlayer->getViewAngle()
    ));
    glm::vec3 vCross = glm::cross(vCurrent, vTarget);
    float angle = glm::angle(vCurrent, vTarget) * Settings::Aimbot::moveRatio;
    glm::vec3 vAim = glm::rotate(vCurrent, angle, vCross);
    glm::vec3 aimViewAngle = Module::game->orientationToViewAngle(vAim);
    localPlayer->setViewAngle(aimViewAngle);
}
