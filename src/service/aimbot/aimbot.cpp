#include <vector>
#include <functional>
#include <optional>
#include <glm/glm.hpp>
#include "module.h"
#include "game/player_interface.h"
#include "aimbot_helper.h"
#include "aimbot.h"

namespace Settings::Aimbot {
    bool on = true;

    // max angle between local player orientation and aim target position
    // range (degree) : (0, 180)
    float maxAngle = glm::radians(10.f);

    //  how fast we aim to a target
    // range : (0, 1]
    // 1 : aim instantly
    // 0.5 : aim 50% to the target in a frame
    // 0.25 : aim slowly than 0.5, etc
    float moveRatio = 1.f;

    // strategy to trigger aimbot
    // trigger on left button is good for automatic weapons
    // trigger on right button is good for sniper, hand gun, etc
    Strategy strategy = Strategy::triggerOnLeftButton;

    // if we don't know the bone position, we cannot use bone aimer
    // so the aim position is between player's foot and top,
    // we use nonBoneAimerRelativeHeight to determine the aim position
    bool useBoneAimer = true;

    // if the useBoneAimer is true
    // aimbot will aim at the given bone position
    Bone bone = Bone::head;

    // the relative height means a position between foot and top
    // range : [0, 1]
    // 0 : aim at foot
    // 0.5 : aim at middle
    // 1 : aim at top
    float nonBoneAimerRelativeHeight = 0.7f;
}

// TODO: add bone aimbot and traceline collision detection aimbot policy

void Aimbot::callback() {

    if (!Settings::Aimbot::on) {
        return;
    }

    // get a strategy
    AimbotHelper::Strategy strategy;
    switch (Settings::Aimbot::strategy) {
        case Settings::Aimbot::Strategy::triggerOnLeftButton:
            strategy = AimbotHelper::triggerOnLeftButton;
            break;
        case Settings::Aimbot::Strategy::triggerOnRightButton:
            strategy = AimbotHelper::triggerOnRightButton;
            break;
    }

    // if not triggered, clear target
    if (!strategy.trigger()) {
        optAimbotTarget = std::nullopt;
        return;
    }

    std::shared_ptr<PlayerInterface> localPlayer = Module::game->getLocalPlayer();

    std::vector<std::shared_ptr<PlayerInterface>> players = Module::game->getPlayers();

    if (players.empty()) {
        return;
    }

    // if there's no aimbot target, find one and aim to it
    std::optional<std::shared_ptr<PlayerInterface>> optEnemy;
    if (optAimbotTarget && (*optAimbotTarget)->getHealth() > 0) {
        optEnemy = optAimbotTarget;
    } else {
        optEnemy = strategy.targetPicker(localPlayer, players);
        optAimbotTarget = optEnemy;
    }

    // aim to it
    if (optEnemy) {
        strategy.aimer(localPlayer, *optEnemy);
    }
}
