#include <vector>
#include <functional>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <windows.h>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "type.h"
#include "aimbot.h"

namespace Settings::Aimbot {
    bool on = true;
    float maxAngle = glm::radians(10.f);
    Policy policy = Policy::minAngle;
}

static float getDeltaAngle(Vec3 o1, Vec3 o2) {
    glm::vec3 v1{o1.x, o1.y, o1.z};
    glm::vec3 v2{o2.x, o2.y, o2.z};
    return glm::angle(glm::normalize(v1), glm::normalize(v2));
}

static std::optional<std::shared_ptr<PlayerBasicInterface>> minAnglePolicy(
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

// TODO: add bone aimbot and traceline collision detection aimbot policy

Aimbot &Aimbot::getInstance() {
    static Aimbot instance;
    return instance;
}

void Aimbot::aimHead(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::shared_ptr<PlayerBasicInterface> targetPlayer
) {
    Vec3 targetPos = targetPlayer->getCameraPosition();
    Vec3 localPos = localPlayer->getCameraPosition();
    Vec3 orientation = targetPos - localPos;
    Vec3 viewAngle = Game::getInstance().orientationToViewAngle(orientation);
    localPlayer->setViewAngle(viewAngle);
}

void Aimbot::callback() {

    if (!Settings::Aimbot::on) {
        return;
    }

    // if aimbot is not triggered, do nothing
    if (!(GetKeyState(VK_RBUTTON) & 0x8000)) {
        aimbotTarget = std::nullopt;
        return;
    }

    std::shared_ptr<PlayerBasicInterface> localPlayer = Game::getInstance().getLocalPlayer();
    std::vector<std::shared_ptr<PlayerBasicInterface>> players = Game::getInstance().getPlayers();
    if (players.empty()) {
        return;
    }

    // choose 'getBestEnemy' function
    std::function<decltype(minAnglePolicy)> getBestEnemy;
    switch (Settings::Aimbot::policy) {
        case Settings::Aimbot::Policy::minAngle:
            getBestEnemy = minAnglePolicy;
            break;
    }

    // if there's no aimbot target, find one and aim to it
    std::optional<std::shared_ptr<PlayerBasicInterface>> enemy;
    if (aimbotTarget && (*aimbotTarget)->getHealth() > 0) {
        enemy = aimbotTarget;
    } else {
        enemy = getBestEnemy(localPlayer, players);
        aimbotTarget = enemy;
    }
    if (enemy) {
        aimHead(localPlayer, *enemy);
    }
}
