#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include "module.h"
#include "game/player_interface.h"
#include "aimbot_strategy.h"
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
    Strategy strategy = {
            AimbotStrategy::leftOrRightKeyTrigger,
            AimbotStrategy::minAnglePicker,
            AimbotStrategy::smoothAimer
    };

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

    bool ignoreInvisiblePlayer = true;

    // for predict aimer
    float dt = 0.01f;
    float gravity = 9.8f;
    bool showPredictAimPosition = true;
}

// TODO: add bone aimbot and traceline collision detection aimbot policy

std::string Aimbot::getName() {
    return "aimbot";
}

void Aimbot::menuCallback() {
    const char *aimbotBone[] = {
            "head",
            "neck",
            "left shoulder",
            "right shoulder",
            "left elbow",
            "right elbow",
            "left hand",
            "right hand",
            "spine",
            "hip",
            "left hip",
            "right hip",
            "left knee",
            "right knee",
            "left foot",
            "right foot",
    };
    ImGui::Checkbox("aimbot", &Settings::Aimbot::on);
    ImGui::SliderAngle(
            "max angle",
            &Settings::Aimbot::maxAngle,
            0.f,
            180.f
    );
    ImGui::SliderFloat(
            "move ratio",
            &Settings::Aimbot::moveRatio,
            0.05,
            1.f
    );
    ImGui::Checkbox("bone aimer", &Settings::Aimbot::useBoneAimer);
    ImGui::Combo(
            "aim at",
            (int *) &Settings::Aimbot::bone,
            aimbotBone,
            IM_ARRAYSIZE(aimbotBone)
    );
    ImGui::SliderFloat(
            "relative height",
            &Settings::Aimbot::nonBoneAimerRelativeHeight,
            0.f,
            1.f
    );
}

void Aimbot::serviceCallback() {

    if (!Settings::Aimbot::on) {
        return;
    }

    const auto &strategy = Settings::Aimbot::strategy;

    // if not triggered, clear target
    if (!strategy.trigger()) {
        aimbotTarget = nullptr;
        return;
    }

    EntityContainer container = Module::game->getEntities();
    std::shared_ptr<PlayerInterface> &localPlayer = container.localPlayer;
    std::vector<std::shared_ptr<PlayerInterface>> &players = container.players;

    if (players.empty()) {
        return;
    }

    // if there's no aimbot target, find one and aim to it
    std::shared_ptr<PlayerInterface> enemy;
    if (aimbotTarget && aimbotTarget->getHealth() > 0) {
        enemy = aimbotTarget;
    } else {
        enemy = strategy.targetPicker(localPlayer, players);
        aimbotTarget = enemy;
    }

    // aim to it
    if (enemy) {
        strategy.aimer(localPlayer, enemy);
    }
}
