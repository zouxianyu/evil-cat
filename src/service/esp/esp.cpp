#include <iostream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "world_to_screen/world_to_screen.h"
#include "esp.h"

namespace Settings::Esp {
    bool on = true;
    BoxType boxType = BoxType::show2D;
    bool showViewLine = true;
    bool showHeadBar = true;
    bool showHeadCircle = true;
    bool showDistance = true;

    ImColor teammateColor = ImColor(0, 255, 0, 225);
    ImColor enemyColor = ImColor(255, 0, 0, 225);

    float barMoveUp = 15.f;
    float barWidth = 125.f;
    float barHeight = 35.f;

    ImColor nameBackgroundColor = ImColor(0, 0, 0, 200);
    ImColor healthBackgroundColor = ImColor(50, 50, 50, 255);
    ImColor healthFullColor = ImColor(255, 255, 255, 255);
    ImColor healthHalfColor = ImColor(150, 150, 150, 255);
    ImColor healthLowColor = ImColor(255, 120, 120, 255);
}

void Esp::callback() {
    if (!Settings::Esp::on) {
        return;
    }

    // get the new view projection matrix and window size of the frame
    WorldToScreen::getInstance().refresh();

    // get players
    std::shared_ptr<PlayerInterface> localPlayer = Module::game->getLocalPlayer();

    std::vector<std::shared_ptr<PlayerInterface>> players = Module::game->getPlayers();

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

        glm::vec3 head = player->getCameraPosition();
        glm::vec3 feet = player->getPosition();
        ImColor boxColor = player->getTeamId() == localPlayer->getTeamId() ?
                           Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show box 2d
        std::optional<glm::vec2> screenHead =
                WorldToScreen::getInstance().translate(head);

        std::optional<glm::vec2> screenFeet =
                WorldToScreen::getInstance().translate(feet);

        if (!screenHead || !screenFeet) {
            continue;
        }

        float height = screenFeet->y - screenHead->y;
        float width = height / 2;


        ImGui::GetBackgroundDrawList()->AddRect(
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

        glm::vec3 head = player->getCameraPosition();
        glm::vec3 feet = player->getPosition();
        glm::vec3 viewAngle = player->getViewAngle();
        glm::vec3 orientation = Module::game->viewAngleToOrientation(viewAngle);
        auto boxColor = player->getTeamId() == localPlayer->getTeamId() ?
                        Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show box 3d
        float height = head.z - feet.z;
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

        glm::mat4 esp3dRotation = glm::rotate(
                glm::mat4(1.0f),
                glm::angle(orientation2d, glm::vec3(1.f, 0.f, 0.f)),
                glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glm::mat4 esp3dTranslate = glm::translate(glm::mat4(1.0f), feet);
        for (auto &v: corners) {
            v = esp3dTranslate * esp3dRotation * v;
            v /= v.w;
        }

        // translate to screen coordination
        bool notShow = false;
        for (int j = 0; j < 8; j++) {
            std::optional<glm::vec2> result = WorldToScreen::getInstance().translate(
                    corners[j]
            );
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

            ImGui::GetBackgroundDrawList()->AddLine(
                    ImVec2(groundP1.x, groundP1.y),
                    ImVec2(groundP2.x, groundP2.y),
                    boxColor
            );

            ImGui::GetBackgroundDrawList()->AddLine(
                    ImVec2(topP1.x, topP1.y),
                    ImVec2(topP2.x, topP2.y),
                    boxColor
            );

            ImGui::GetBackgroundDrawList()->AddLine(
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

        glm::vec3 head = player->getCameraPosition();
        glm::vec3 viewAngle = player->getViewAngle();
        glm::vec3 orientation = Module::game->viewAngleToOrientation(viewAngle);
        glm::vec3 viewLineEnd = head + orientation;
        auto viewLineColor = player->getTeamId() == localPlayer->getTeamId() ?
                             Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show view line
        std::optional<glm::vec2> screenHead =
                WorldToScreen::getInstance().translate(head);

        std::optional<glm::vec2> screenViewLineEnd =
                WorldToScreen::getInstance().translate(viewLineEnd);

        if (!screenHead || !screenViewLineEnd) {
            continue;
        }

        ImGui::GetBackgroundDrawList()->AddLine(
                ImVec2(screenHead->x, screenHead->y),
                ImVec2(screenViewLineEnd->x, screenViewLineEnd->y),
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
        glm::vec3 head = player->getCameraPosition();
        glm::vec3 feet = player->getPosition();

        std::optional<glm::vec2> screenHead =
                WorldToScreen::getInstance().translate(head);

        std::optional<glm::vec2> screenFeet =
                WorldToScreen::getInstance().translate(feet);

        if (!screenHead || !screenFeet) {
            continue;
        }

        float headRadius = (screenFeet->y - screenHead->y) / 12.f;

        ImGui::GetBackgroundDrawList()->AddCircle(
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
        glm::vec3 top = {feet.x, feet.y, feet.z + player->getHeight()};

        // calculate the head bar position
        std::optional<glm::vec2> screenTop =
                WorldToScreen::getInstance().translate(top);

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

        auto barBorderColor = player->getTeamId() == localPlayer->getTeamId() ?
                              Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        ImGui::GetBackgroundDrawList()->AddRect(
                ImVec2(barLeftTop.x, barLeftTop.y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                barBorderColor
        );

        // draw name lockHealth bar
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

        ImGui::GetBackgroundDrawList()->AddRectFilled(
                ImVec2(upperAreaLeftTop.x, upperAreaLeftTop.y),
                ImVec2(upperAreaRightBottom.x, upperAreaRightBottom.y),
                Settings::Esp::nameBackgroundColor
        );

        ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(upperAreaLeftTop.x, upperAreaLeftTop.y),
                ImColor(255,255,255),
                player->getName().c_str()
        );

        ImGui::GetBackgroundDrawList()->AddRectFilled(
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
        ImGui::GetBackgroundDrawList()->AddRectFilled(
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

        std::optional<glm::vec2> screenFeet =
                WorldToScreen::getInstance().translate(feet);

        if (!screenFeet) {
            continue;
        }

        auto pos1 = localPlayer->getPosition();
        auto pos2 = player->getPosition();
        auto distance = (float) sqrt(
                pow(pos1.x - pos2.x, 2) +
                pow(pos1.y - pos2.y, 2) +
                pow(pos1.z - pos2.z, 2)
        );
        std::string distanceStr = std::to_string((int) distance) + "m";
        ImVec2 textSize = ImGui::CalcTextSize(distanceStr.c_str());
        ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screenFeet->x - textSize.x / 2, screenFeet->y - textSize.y),
                ImColor(255, 255, 255),
                distanceStr.c_str()
        );
    }
}
