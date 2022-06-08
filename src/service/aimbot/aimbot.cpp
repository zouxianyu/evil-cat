#include <vector>
#include <functional>
#include <optional>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <windows.h>
#include "type.h"
#include "settings.h"
#include "entity/entity_manager.h"
#include "service/esp/esp.h"
#include "aimbot.h"

static double getDeltaAngle(Vec3 o1, Vec3 o2) {
    glm::vec3 v1{o1.x, o1.y, o1.z};
    glm::vec3 v2{o2.x, o2.y, o2.z};
    return glm::angle(glm::normalize(v1),glm::normalize(v2));
}

static std::optional<Player> minAnglePolicy(Player &localPlayer, std::vector<Player> &players) {

    // get local player orientation
    Vec3 localPlayerViewAngle = localPlayer.getViewAngle();
    Vec3 localPlayerOrientation = Esp::getInstance().viewAngleToOrientation(localPlayerViewAngle);

    // find the best enemy
    // maybe there's no valid enemy, so we use "optional" container
    std::optional<Player> bestEnemy = std::nullopt;
    double minAngle = glm::pi<double>();

    for (auto &player: players) {

        // if the player is not a valid enemy (death or teammates)
        if (player.getHealth() <= 0 || player.getTeam() == localPlayer.getTeam()) {
            continue;
        }

        // get target orientation
        Vec3 targetOrientation = player.getHead() - localPlayer.getHead();

        // calculate delta view angle
        double deltaAngle = getDeltaAngle(localPlayerOrientation, targetOrientation);

        // make sure the enemy's delta angle is less than the threshold
        if (deltaAngle >= Settings::getInstance().aimbotMaxAngle) {
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

using AimbotPolicy = std::function<std::optional<Player>(Player &, std::vector<Player> &)>;

static AimbotPolicy getBestEnemy = AIMBOT_POLICY;

Aimbot &Aimbot::getInstance() {
    static Aimbot instance;
    return instance;
}

void Aimbot::aimHead(Player &localPlayer, Player &enemy) {
    Vec3 playerPos = enemy.getHead();
    Vec3 localPos = localPlayer.getHead();
    Vec3 orientation = playerPos - localPos;
    Vec3 viewAngle = Esp::getInstance().orientationToViewAngle(orientation);
    localPlayer.setViewAngle(viewAngle);
}

void Aimbot::callback() {

    // if aimbot is not triggered, do nothing
    if (!(GetKeyState(VK_RBUTTON) & 0x8000)) {
        aimbotTarget = std::nullopt;
        return;
    }

    Player localPlayer = EntityManager::getInstance().getLocalPlayer();
    std::vector<Player> players = EntityManager::getInstance().getPlayers();
    if (players.empty()) {
        return;
    }

    std::optional<Player> enemy;
    if (aimbotTarget.has_value()
        && aimbotTarget.value().getHealth() > 0) {
        enemy = aimbotTarget.value();
    }else {
        enemy = getBestEnemy(localPlayer, players);
        aimbotTarget = enemy;
    }
    if (enemy.has_value()) {
        aimHead(localPlayer, enemy.value());
    }
}
