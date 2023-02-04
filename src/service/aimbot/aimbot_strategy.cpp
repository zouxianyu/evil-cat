#include <algorithm>
#include <utility>
#include <vector>
#include <memory>
#include <chrono>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <windows.h>
#include "game/weapon.h"
#include "aimbot.h"
#include "module.h"
#include "aimbot_strategy.h"
#include "settings.h"

#include "world_to_screen/world_to_screen.h"

namespace AimbotStrategy::Detail {
    float getDeltaAngle(glm::vec3 v1, glm::vec3 v2);

    glm::vec3 getAimPosition(const std::shared_ptr<PlayerInterface>& player);
}

using namespace AimbotStrategy::Detail;

float AimbotStrategy::Detail::getDeltaAngle(glm::vec3 v1, glm::vec3 v2) {
    return glm::angle(glm::normalize(v1), glm::normalize(v2));
}

glm::vec3 AimbotStrategy::Detail::getAimPosition(const std::shared_ptr<PlayerInterface>& player) {
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

        // filter by visibility
        if (Settings::Aimbot::ignoreInvisiblePlayer && !player->isVisible()) {
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

void AimbotStrategy::smoothAimer(
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

struct PositionInfo {
    glm::vec3 position;
    std::chrono::time_point<std::chrono::system_clock> time;
};

static glm::vec3 linearPredictor(
        std::shared_ptr<PlayerInterface> targetPlayer,
        float time
) {
    static std::shared_ptr<PlayerInterface> lastPlayer = targetPlayer;
    static std::deque<PositionInfo> sequence;

    if (lastPlayer != targetPlayer) {
        sequence.clear();
        lastPlayer = targetPlayer;
    }

    glm::vec3 position = getAimPosition(targetPlayer);

    sequence.emplace_front(PositionInfo{position, std::chrono::system_clock::now()});
    if (sequence.size() > 10) {
        sequence.pop_back();
    }
    if (sequence.size() < 3) {
        return position;
    }

    float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            sequence.front().time - sequence.back().time).count() / 1000.0f;
    glm::vec3 deltaPosition = sequence.front().position - sequence.back().position;
    return position + (deltaPosition / deltaTime * time);
}

struct CurveSolver {
    std::vector<CurveKey> keys;

    explicit CurveSolver(std::vector<CurveKey> curve)
            : keys(std::move(curve)) {
        std::sort(keys.begin(), keys.end(),
                  [](const CurveKey &a, const CurveKey &b) {
                      return a.first < b.first;
                  });
    }

    bool valid() const {
        return !keys.empty();
    }

    float get(float x) const {
        if (!valid()) {
            return 0.f;
        }

        if (x <= keys.front().first) {
            return keys.front().second;
        }
        if (x >= keys.back().first) {
            return keys.back().second;
        }

        for (int i = 0; i < keys.size() - 1; ++i) {
            if (x >= keys[i].first && x <= keys[i + 1].first) {
                float t = (x - keys[i].first) / (keys[i + 1].first - keys[i].first);
                return keys[i].second + (keys[i + 1].second - keys[i].second) * t;
            }
        }

        return 0.f;
    }
};

struct BulletInfo {
    glm::vec3 position;
    float travelTime;
};

static BulletInfo calculateBulletInfo(
        const Curve& speedCurve,
        glm::vec3 position,
        glm::vec3 orientation,
        float distance
) {
    const float dt = Settings::Aimbot::dt;
    const float gravity = Settings::Aimbot::gravity;

    CurveSolver curve(speedCurve);
    if (!curve.valid()) {
        return {};
    }

    int maxTry = 500;

    float x = 0.f;
    float t = 0.f;
    glm::vec3 currPos = position;
    glm::vec3 currDir = orientation;
    glm::vec3 currVel{};
    glm::vec3 deltaPos{};
    glm::vec3 vGravity = {0.f, 0.f, -gravity};

    do {
        currVel = currDir * curve.get(x);
        deltaPos = currVel * dt;

        x += glm::length(deltaPos);
        t += dt;

        currPos += deltaPos;
        currDir = glm::normalize(currVel + vGravity * dt);
    } while (--maxTry && x < distance);

    float exceed = (x - distance) / glm::length(deltaPos);
    currPos -= deltaPos * exceed;
    t -= dt * exceed;
    return {currPos, t};
}

static glm::vec3 getZeroDistanceOffset(const Curve &speedCurve, float zeroDistance) {
    BulletInfo bullet = calculateBulletInfo(
            speedCurve,
            {0.f, 0.f, 0.f},
            {1.f, 0.f, 0.f},
            zeroDistance
    );

    return {0.f, 0.f, -bullet.position.z};
}

static bool isValidWeapon(const Weapon &weapon) {
    return !weapon.speedCurve.empty();
}

void AimbotStrategy::predictAimer(
        std::shared_ptr<PlayerInterface> localPlayer,
        std::shared_ptr<PlayerInterface> targetPlayer
) {
    // get basic info
    Weapon weapon = localPlayer->getWeapon();
    if (!isValidWeapon(weapon)) {
        return smoothAimer(localPlayer, targetPlayer);
    }

    glm::vec3 localPosition = localPlayer->getCameraPosition();
    glm::vec3 aimPosition = getAimPosition(targetPlayer);
    glm::vec3 vTargetNotFixed = glm::normalize(aimPosition - localPosition);
    float distance = glm::distance(aimPosition, localPosition) * Settings::distanceFactor;

    // calculate bullet drop position and travel time
    // notice: unit = meter
    BulletInfo bullet = calculateBulletInfo(
            weapon.speedCurve,
            localPosition * Settings::distanceFactor,
            vTargetNotFixed,
            distance
    );
    bullet.position /= Settings::distanceFactor;
    glm::vec3 zeroDistanceOffset = getZeroDistanceOffset(weapon.speedCurve, weapon.zeroDistance)
                                           /= Settings::distanceFactor;

    // get predict aim position
    glm::vec3 predictPlayerPosition = linearPredictor(targetPlayer, bullet.travelTime);
    glm::vec3 predictAimPosition = (aimPosition - bullet.position) - zeroDistanceOffset +
                                   predictPlayerPosition;
    glm::vec3 vPredictAim = glm::normalize(predictAimPosition - localPosition);

    // show predict aim position on screen
    if (Settings::Aimbot::showPredictAimPosition) {
        glm::vec2 screenPosition = WorldToScreen::translate(predictAimPosition)
                .value_or(glm::vec2(0, 0));
        Module::view->drawCircle(
                ImVec2(screenPosition.x, screenPosition.y),
                5.f,
                ImColor(255, 0, 0, 255)
        );
    }

    // aim to it (with move ratio)
    glm::vec3 vCurrent = glm::normalize(Module::game->viewAngleToOrientation(
            localPlayer->getViewAngle()
    ));
    glm::vec3 vCross = glm::cross(vCurrent, vPredictAim);
    float angle = glm::angle(vCurrent, vPredictAim) * Settings::Aimbot::moveRatio;
    glm::vec3 vAim = glm::rotate(vCurrent, angle, vCross);
    glm::vec3 aimViewAngle = Module::game->orientationToViewAngle(vAim);
    localPlayer->setViewAngle(aimViewAngle);
}
