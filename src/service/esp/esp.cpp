#include <iostream>
#include <vector>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "world_to_screen/world_to_screen.h"
#include "magic_enum.h"
#include "settings.h"
#include "esp.h"

namespace Settings::Esp {
    bool on = true;
    BoxType boxType = BoxType::show3D;
    bool showBone = false;
    bool showViewLine = true;
    bool showHeadBar = true;
    bool showHeadCircle = false;
    bool showDistance = true;
    bool colorfulEnemyHeadBar = false;

    ImColor teammateColor = ImColor(0, 255, 0, 225);
    ImColor enemyColor = ImColor(255, 0, 0, 225);
    ImColor boneColor = ImColor(234, 240, 68, 255);

    float barMoveUp = 15.f;
    float barWidth = 125.f;
    float barHeight = 35.f;

    ImColor nameBackgroundColor = ImColor(0, 0, 0, 200);
    ImColor healthBackgroundColor = ImColor(50, 50, 50, 255);
    ImColor healthFullColor = ImColor(255, 255, 255, 255);
    ImColor healthHalfColor = ImColor(150, 150, 150, 255);
    ImColor healthLowColor = ImColor(255, 120, 120, 255);

    std::vector<ImColor> teamColors = {
            ImColor::HSV(1.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(2.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(3.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(4.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(5.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(6.5f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(10.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(11.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(12.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(13.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(14.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(15.0f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(17.5f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(18.5f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(19.5f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(20.5f / 24.0f, 1.0f, 0.8f),
            ImColor::HSV(22.0f / 24.0f, 1.0f, 0.8f),

            ImColor::HSV(1.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(2.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(3.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(4.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(5.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(6.5f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(10.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(11.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(12.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(13.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(14.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(15.0f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(17.5f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(18.5f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(19.5f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(20.5f / 24.0f, 0.5f, 0.8f),
            ImColor::HSV(22.0f / 24.0f, 0.5f, 0.8f),

            ImColor::HSV(1.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(2.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(3.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(4.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(5.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(6.5f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(10.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(11.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(12.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(13.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(14.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(15.0f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(17.5f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(18.5f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(19.5f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(20.5f / 24.0f, 0.5f, 0.6f),
            ImColor::HSV(22.0f / 24.0f, 0.5f, 0.6f),
    };
}

std::string Esp::getName() {
    return "ESP";
}

void Esp::menuCallback() {
    const char *boxType[] = { "2D", "3D" };
    ImGui::Checkbox("ESP", &Settings::Esp::on);
    ImGui::Combo(
            "box type",
            (int *) &Settings::Esp::boxType,
            boxType,
            IM_ARRAYSIZE(boxType)
    );
    ImGui::Checkbox("bone", &Settings::Esp::showBone);
    ImGui::Checkbox("view line", &Settings::Esp::showViewLine);
    ImGui::Checkbox("head bar", &Settings::Esp::showHeadBar);
    ImGui::Checkbox("head circle", &Settings::Esp::showHeadCircle);
    ImGui::Checkbox("distance", &Settings::Esp::showDistance);
    ImGui::Separator();
    ImGui::ColorEdit4("teammate", (float *)&Settings::Esp::teammateColor.Value);
    ImGui::ColorEdit4("enemy",(float *)&Settings::Esp::enemyColor.Value);
    ImGui::ColorEdit4("full health",(float *)&Settings::Esp::healthFullColor.Value);
    ImGui::ColorEdit4("half health",(float *)&Settings::Esp::healthHalfColor.Value);
    ImGui::ColorEdit4("low health",(float *)&Settings::Esp::healthLowColor.Value);
}

void Esp::serviceCallback() {
    if (!Settings::Esp::on) {
        return;
    }

    // get players
    EntityContainer container = Module::game->getEntities();
    std::shared_ptr<PlayerInterface> &localPlayer = container.localPlayer;
    std::vector<std::shared_ptr<PlayerInterface>> &players = container.players;

    // show bone on the bottom
    if (Settings::Esp::showBone) {
        showBone(localPlayer, players);
    }

    // show esp box
    switch (Settings::Esp::boxType) {
        case Settings::Esp::BoxType::show2D:
            showEsp2D(localPlayer, players);
            break;
        case Settings::Esp::BoxType::show3D:
            showEsp3D(localPlayer, players);
            break;
    }

    // show view line
    if (Settings::Esp::showViewLine) {
        showViewLine(localPlayer, players);
    }

    // show head circle
    if (Settings::Esp::showHeadCircle) {
        showHeadCircle(localPlayer, players);
    }

    // show head bar
    if (Settings::Esp::showHeadBar) {
        showHeadBar(localPlayer, players);
    }

    // show distance
    if (Settings::Esp::showDistance) {
        showDistance(localPlayer, players);
    }

}

void Esp::showEsp2D(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>> &players
) {

    for (auto &player: players) {
        // skip the local player and the dead player
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        glm::vec3 feet = player->getPosition();
        glm::vec3 top = feet;
        top.z += player->getHeight();

        ImColor boxColor = player->getTeamId() == localPlayer->getTeamId() ?
                           Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show box 2d
        std::optional<glm::vec2> screenTop = WorldToScreen::translate(top);
        std::optional<glm::vec2> screenFeet = WorldToScreen::translate(feet);

        if (!screenTop || !screenFeet) {
            continue;
        }

        float height = screenFeet->y - screenTop->y;
        float width = height / 2;


        Module::view->drawLine(
                ImVec2(screenFeet->x - width / 2, screenFeet->y - height),
                ImVec2(screenFeet->x + width / 2, screenFeet->y),
                boxColor
        );
    }
}

void Esp::showEsp3D(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>>& players
) {

    for (const auto &player: players) {
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        glm::vec3 feet = player->getPosition();
        glm::vec3 top = feet;
        top.z += player->getHeight();

        glm::vec3 viewAngle = player->getViewAngle();
        glm::vec3 orientation = Module::game->viewAngleToOrientation(viewAngle);
        auto boxColor = player->getTeamId() == localPlayer->getTeamId() ?
                        Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show box 3d
        float height = top.z - feet.z;
        float width = height / 2;
        glm::vec4 corners[8] = {
                {width / 2,  width / 2,  0,      1},
                {width / 2,  -width / 2, 0,      1},
                {-width / 2, -width / 2, 0,      1},
                {-width / 2, width / 2,  0,      1},
                {width / 2,  width / 2,  height, 1},
                {width / 2,  -width / 2, height, 1},
                {-width / 2, -width / 2, height, 1},
                {-width / 2, width / 2,  height, 1},
        };
        glm::vec2 screenCorners[8] = {};

        glm::vec3 orientation2d =
                glm::normalize(glm::vec3(orientation.x, orientation.y, 0));

        glm::vec3 axisX = glm::vec3(1.f, 0.f, 0.f);
        glm::vec3 axisZ = glm::vec3(0.f, 0.f, 1.f);
        glm::mat4 esp3dRotation = glm::rotate(
                glm::mat4(1.0f),
                glm::orientedAngle(axisX, orientation2d, axisZ),
                axisZ
        );
        glm::mat4 esp3dTranslate = glm::translate(glm::mat4(1.0f), feet);
        for (auto &v: corners) {
            v = esp3dTranslate * esp3dRotation * v;
            v /= v.w;
        }

        // translate to screen coordination
        bool notShow = false;
        for (int j = 0; j < 8; j++) {
            std::optional<glm::vec2> result = WorldToScreen::translate(corners[j]);
            if (!result) {
                notShow = true;
                break;
            }
            screenCorners[j] = *result;
        }

        // if the player cannot fully show on the screen, skip it
        if (notShow) {
            continue;
        }

        //show box 3d
        for (int j = 0; j < 4; j++) {
            const glm::vec2 &groundP1 = screenCorners[j];
            const glm::vec2 &groundP2 = screenCorners[(j + 1) % 4];
            const glm::vec2 &topP1 = screenCorners[j + 4];
            const glm::vec2 &topP2 = screenCorners[4 + (j + 1) % 4];

            Module::view->drawLine(
                    ImVec2(groundP1.x, groundP1.y),
                    ImVec2(groundP2.x, groundP2.y),
                    boxColor
            );

            Module::view->drawLine(
                    ImVec2(topP1.x, topP1.y),
                    ImVec2(topP2.x, topP2.y),
                    boxColor
            );

            Module::view->drawLine(
                    ImVec2(groundP1.x, groundP1.y),
                    ImVec2(topP1.x, topP1.y),
                    boxColor
            );
        }
    }
}

void Esp::showViewLine(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>>& players
) {

    for (const auto &player: players) {
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        glm::vec3 viewAngle = player->getViewAngle();
        glm::vec3 orientation = Module::game->viewAngleToOrientation(viewAngle);
        glm::vec3 orientation2D = glm::normalize(glm::vec3(orientation.x, orientation.y, 0));

        glm::vec3 pMiddle = player->getPosition(); pMiddle.z += player->getHeight() * 0.5f;
        glm::vec3 pFront = pMiddle + orientation2D * player->getHeight() * 0.25f;
        glm::vec3 pTop = pFront; pTop.z += player->getHeight() * 0.5f;
        glm::vec3 pDown = pFront; pDown.z -= player->getHeight() * 0.5f;
        glm::vec3 pDest = pFront + orientation * player->getHeight() * 0.5f;

        auto viewLineColor = player->getTeamId() == localPlayer->getTeamId() ?
                             Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        std::optional<glm::vec2> screenTop = WorldToScreen::translate(pTop);
        std::optional<glm::vec2> screenDown = WorldToScreen::translate(pDown);
        std::optional<glm::vec2> screenDest = WorldToScreen::translate(pDest);

        if (!screenTop || !screenDown || !screenDest) {
            continue;
        }

        Module::view->drawLine(
                ImVec2(screenDest->x, screenDest->y),
                ImVec2(screenTop->x, screenTop->y),
                viewLineColor
        );
        Module::view->drawLine(
                ImVec2(screenDest->x, screenDest->y),
                ImVec2(screenDown->x, screenDown->y),
                viewLineColor
        );
    }
}

void Esp::showHeadCircle(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>>& players
) {
    for (const auto &player: players) {
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        auto headCircleColor = player->getTeamId() == localPlayer->getTeamId() ?
                               Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show head circle
        glm::vec3 head = player->getBonePositions()[static_cast<int>(Bone::head)];
        glm::vec3 feet = player->getPosition();

        std::optional<glm::vec2> screenHead = WorldToScreen::translate(head);
        std::optional<glm::vec2> screenFeet = WorldToScreen::translate(feet);

        if (!screenHead || !screenFeet) {
            continue;
        }

        // if we want to calculate the head radius
        // we should get the player height on screen
        // and divide it by a constant
        float headRadius = (screenFeet->y - screenHead->y) / 12.f;

        Module::view->drawCircle(
                ImVec2(screenHead->x, screenHead->y),
                headRadius,
                headCircleColor
        );
    }
}

void Esp::showHeadBar(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>>& players
) {
    for (const auto &player: players) {
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        glm::vec3 feet = player->getPosition();
        glm::vec3 top = feet;
        top.z += player->getHeight();

        // calculate the head bar position
        std::optional<glm::vec2> screenTop = WorldToScreen::translate(top);

        if (!screenTop) {
            continue;
        }

        // draw the head bar border
        glm::vec2 barLeftTop = {
                screenTop->x - Settings::Esp::barWidth / 2,
                screenTop->y - (Settings::Esp::barHeight + Settings::Esp::barMoveUp)
        };
        glm::vec2 barRightBottom = {
                screenTop->x + Settings::Esp::barWidth / 2,
                screenTop->y - Settings::Esp::barMoveUp
        };

        ImColor barBorderColor;
        if (player->getTeamId() == localPlayer->getTeamId()) {
            barBorderColor = Settings::Esp::teammateColor;
        } else {
            if (Settings::Esp::colorfulEnemyHeadBar) {
                auto &teamColors = Settings::Esp::teamColors;
                barBorderColor = teamColors[player->getTeamId() % teamColors.size()];
            } else {
                barBorderColor = Settings::Esp::enemyColor;
            }
        }

        Module::view->drawRect(
                ImVec2(barLeftTop.x, barLeftTop.y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                barBorderColor
        );

        // draw name health bar
        glm::vec2 upperAreaLeftTop = {
                barLeftTop.x + 1,
                barLeftTop.y + 1
        };
        glm::vec2 upperAreaRightBottom = {
                barLeftTop.x + Settings::Esp::barWidth - 1,
                barLeftTop.y + Settings::Esp::barHeight * 0.64f - 1
        };
        glm::vec2 lowerAreaLeftTop = {
                barLeftTop.x,
                barLeftTop.y + Settings::Esp::barHeight * 0.64f
        };
        glm::vec2 lowerAreaRightBottom = {
                barRightBottom.x - 1,
                barRightBottom.y - 1
        };

        Module::view->drawRectFilled(
                ImVec2(upperAreaLeftTop.x, upperAreaLeftTop.y),
                ImVec2(upperAreaRightBottom.x, upperAreaRightBottom.y),
                Settings::Esp::nameBackgroundColor
        );

        Module::view->drawString(
                ImVec2(upperAreaLeftTop.x, upperAreaLeftTop.y),
                ImColor(255,255,255),
                player->getName()
        );

        Module::view->drawRectFilled(
                ImVec2(lowerAreaLeftTop.x, lowerAreaLeftTop.y),
                ImVec2(lowerAreaRightBottom.x, lowerAreaRightBottom.y),
                Settings::Esp::healthBackgroundColor
        );

        glm::vec2 lowerInnerLeftTop = {
                lowerAreaLeftTop.x + 1,
                lowerAreaLeftTop.y + 1
        };
        glm::vec2 lowerInnerRightBottom = {
                lowerAreaRightBottom.x - 1,
                lowerAreaRightBottom.y - 1
        };

        float health = player->getHealth();
        float fullHealthLength = lowerInnerRightBottom.x - lowerInnerLeftTop.x;
        float healthLength = fullHealthLength * health / 100.f;
        glm::vec2 healthLeftTop = {
                lowerInnerLeftTop.x,
                lowerInnerLeftTop.y
        };
        glm::vec2 healthRightBottom = {
                lowerInnerLeftTop.x + healthLength,
                lowerInnerRightBottom.y
        };
        ImColor healthBarColor;
        if (health >= 60) {
            healthBarColor = Settings::Esp::healthFullColor;
        } else if (health >= 30) {
            healthBarColor = Settings::Esp::healthHalfColor;
        } else {
            healthBarColor = Settings::Esp::healthLowColor;
        }
        Module::view->drawRectFilled(
                ImVec2(healthLeftTop.x, healthLeftTop.y),
                ImVec2(healthRightBottom.x, healthRightBottom.y),
                healthBarColor
        );
    }
}

void Esp::showDistance(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>>& players
) {
    for (const auto &player: players) {
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        glm::vec3 feet = player->getPosition();

        std::optional<glm::vec2> screenFeet = WorldToScreen::translate(feet);

        if (!screenFeet) {
            continue;
        }

        auto distance = glm::length(player->getPosition() - localPlayer->getPosition()) *
                Settings::distanceFactor;
        std::string distanceStr = std::to_string((int)std::round(distance)) + "m";
        // TODO: get string size via View interface
        ImVec2 textSize(distanceStr.length() * 11.f, 22.f);
        Module::view->drawString(
                ImVec2(screenFeet->x - textSize.x / 2, screenFeet->y - textSize.y),
                ImColor(255, 255, 255),
                distanceStr
        );
    }
}

inline static ImVec2 glmToImVec2(const glm::vec2& vec) {
    return ImVec2(vec.x, vec.y);
}

void Esp::showBone(
        const std::shared_ptr<PlayerInterface>& localPlayer,
        const std::vector<std::shared_ptr<PlayerInterface>> &players
) {

    for (auto &player: players) {
        // skip the local player and the dead player
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        // get all bone positions
        bool showPlayer = true;
        BoneArray bonePositions = player->getBonePositions();
        glm::vec2 boneScreenPositions[magic_enum::enum_count<Bone>()];
        for (int i = 0; i < magic_enum::enum_count<Bone>(); i++) {
            std::optional<glm::vec2> screenPosition =
                    WorldToScreen::translate(bonePositions[i]);
            if (!screenPosition) {
                showPlayer = false;
                break;
            }
            boneScreenPositions[i] = *screenPosition;
        }

        // don't show the player if any bone is not visible
        if (!showPlayer) {
            continue;
        }

        // now the bone positions array contains all the bone positions
        // draw them on the screen now

        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::head)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::neck)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::neck)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftShoulder)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::neck)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightShoulder)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::neck)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::spine)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftShoulder)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftElbow)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightShoulder)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightElbow)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftElbow)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftHand)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightElbow)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightHand)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::spine)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::hip)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::hip)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftHip)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::hip)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightHip)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftHip)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftKnee)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightHip)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightKnee)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftKnee)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::leftFoot)]),
                Settings::Esp::boneColor
        );
        Module::view->drawLine(
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightKnee)]),
                glmToImVec2(boneScreenPositions[static_cast<int>(Bone::rightFoot)]),
                Settings::Esp::boneColor
        );
    }
}
