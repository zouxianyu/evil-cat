#include <iostream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "service/world_to_screen/world_to_screen.h"
#include "type.h"
#include "esp.h"

namespace Settings::Esp {
    bool on = true;
    BoxType boxType = BoxType::show2D;
    bool showViewLine = true;
    bool showHeadBar = true;
    bool showHeadCircle = true;

    ImColor teammateColor = ImColor(0, 255, 0);
    ImColor enemyColor = ImColor(255, 0, 0);

    float barMoveUp = 15.f;
    float barWidth = 125.f;
    float barHeight = 25.f;

    ImColor nameBackgroundColor = ImColor(0, 0, 0);
    ImColor healthBackgroundColor = ImColor(50, 50, 50);
    ImColor healthFullColor = ImColor(255, 255, 255);
    ImColor healthHalfColor = ImColor(255, 255, 255);
    ImColor healthLowColor = ImColor(255, 255, 255);
}

Esp &Esp::getInstance() {
    static Esp instance;
    return instance;
}

void Esp::callback() {
    if (!Settings::Esp::on) {
        return;
    }

    // get the new view projection matrix and window size of the frame
    WorldToScreen::getInstance().refresh();

    // get players
    std::shared_ptr<PlayerBasicInterface> localPlayer = Game::getInstance().getLocalPlayer();
    std::vector<std::shared_ptr<PlayerBasicInterface>> players = Game::getInstance().getPlayers();

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

    // show self test
//    Player localPlayer = EntityManager::getInstance().getLocalPlayer();
//    Vec3 localHead = localPlayer.getHead();
//    Vec3 localViewAngle = localPlayer.getViewAngle();
//    Vec3 localOrientation = viewAngleToOrientation(localViewAngle);
//    Vec3 viewLineEnd = localHead + localOrientation;
//
//    Vec2 screenViewLineEnd{};
//    if (!WorldToScreen::getInstance().translate(viewLineEnd, screenViewLineEnd)) {
//        return;
//    }
//    ImGui::GetForegroundDrawList()->AddCircle(ImVec2(screenViewLineEnd.x, screenViewLineEnd.y),
//                                              5, ImColor(255, 255, 255, 255));

}

void Esp::showEsp2D(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        const std::vector<std::shared_ptr<PlayerBasicInterface>> &players
) {

    for (auto &player: players) {
        // skip the local player and the dead player
        if (*player == *localPlayer || player->getHealth() <= 0) {
            continue;
        }

        Vec3 head = player->getCameraPosition();
        Vec3 feet = player->getPosition();
        ImColor boxColor = player->getTeamId() == localPlayer->getTeamId() ?
                           Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show box 2d
        std::optional<Vec2> screenHead = WorldToScreen::getInstance().translate(head);
        std::optional<Vec2> screenFeet = WorldToScreen::getInstance().translate(feet);

        if (!screenHead || !screenFeet) {
            continue;
        }

        float height = screenFeet->y - screenHead->y;
        float width = height / 2;


        ImGui::GetForegroundDrawList()->AddRect(
                ImVec2(screenFeet->x - width / 2, screenFeet->y - height),
                ImVec2(screenFeet->x + width / 2, screenFeet->y),
                boxColor
        );
    }
}

void Esp::showEsp3D(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::vector<std::shared_ptr<PlayerBasicInterface>> players
) {

    for (const auto &player: players) {
        if (player->getHealth() <= 0) {
            continue;
        }

        Vec3 head = player->getCameraPosition();
        Vec3 feet = player->getPosition();
        Vec3 viewAngle = player->getViewAngle();
        Vec3 orientation = Game::getInstance().viewAngleToOrientation(viewAngle);
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
        Vec2 screenCorners[8] = {};

        glm::vec3 orientation2d = glm::normalize(glm::vec3(orientation.x, orientation.y, 0));
        glm::mat4 esp3dRotation = glm::rotate(
                glm::mat4(1.0f),
                glm::angle(orientation2d, glm::vec3(1.f, 0.f, 0.f)),
                glm::vec3(0.0f, 0.0f, 1.0f)
        );
        glm::mat4 esp3dTranslate = glm::translate(glm::mat4(1.0f),
                                                  glm::vec3(feet.x, feet.y, feet.z));
        for (auto &v: corners) {
            v = esp3dTranslate * esp3dRotation * v;
            v /= v.w;
        }

        // translate to screen coordination
        bool notShow = false;
        for (int j = 0; j < 8; j++) {
            std::optional<Vec2> result = WorldToScreen::getInstance().translate(
                    Vec3{corners[j].x, corners[j].y, corners[j].z}
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
        for (int j: {0, 1, 2, 3}) {
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenCorners[j].x, screenCorners[j].y),
                                                    ImVec2(screenCorners[(j + 1) % 4].x,
                                                           screenCorners[(j + 1) % 4].y),
                                                    boxColor);
            ImGui::GetForegroundDrawList()->AddLine(
                    ImVec2(screenCorners[4 + j].x, screenCorners[4 + j].y),
                    ImVec2(screenCorners[4 + (j + 1) % 4].x,
                           screenCorners[4 + (j + 1) % 4].y),
                    boxColor);
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenCorners[j].x, screenCorners[j].y),
                                                    ImVec2(screenCorners[j + 4].x,
                                                           screenCorners[j + 4].y),
                                                    boxColor);
        }
    }
}

void Esp::showViewLine(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::vector<std::shared_ptr<PlayerBasicInterface>> players
) {

    for (const auto &player: players) {
        if (player->getHealth() <= 0) {
            continue;
        }

        Vec3 head = player->getCameraPosition();
        Vec3 viewAngle = player->getViewAngle();
        Vec3 orientation = Game::getInstance().viewAngleToOrientation(viewAngle);
        Vec3 viewLineEnd = head + orientation;
        auto viewLineColor = player->getTeamId() == localPlayer->getTeamId() ?
                             Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show view line
        std::optional<Vec2> screenHead = WorldToScreen::getInstance().translate(head);
        std::optional<Vec2> screenViewLineEnd = WorldToScreen::getInstance().translate(viewLineEnd);
        if (!screenHead || !screenViewLineEnd) {
            continue;
        }

        ImGui::GetForegroundDrawList()->AddLine(
                ImVec2(screenHead->x, screenHead->y),
                ImVec2(screenViewLineEnd->x, screenViewLineEnd->y),
                viewLineColor
        );
    }
}

void Esp::showHeadCircle(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::vector<std::shared_ptr<PlayerBasicInterface>> players
) {
    for (const auto &player: players) {
        if (player->getHealth() <= 0) {
            continue;
        }

        auto headCircleColor = player->getTeamId() == localPlayer->getTeamId() ?
                               Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        // show head circle
        Vec3 head = player->getCameraPosition();
        Vec3 feet = player->getPosition();

        std::optional<Vec2> screenHead = WorldToScreen::getInstance().translate(head);
        std::optional<Vec2> screenFeet = WorldToScreen::getInstance().translate(feet);

        if (!screenHead || !screenFeet) {
            continue;
        }

        float headRadius = (screenFeet->y - screenHead->y) / 12.f;

        ImGui::GetForegroundDrawList()->AddCircle(
                ImVec2(screenHead->x, screenHead->y),
                headRadius,
                headCircleColor
        );
    }
}

void Esp::showHeadBar(
        std::shared_ptr<PlayerBasicInterface> localPlayer,
        std::vector<std::shared_ptr<PlayerBasicInterface>> players
) {
    for (const auto &player: players) {
        if (player->getHealth() <= 0) {
            continue;
        }

        Vec3 feet = player->getPosition();
        Vec3 top = {feet.x, feet.y, feet.z + player->getHeight()};

        // calculate the head bar position
        std::optional<Vec2> screenTop = WorldToScreen::getInstance().translate(top);
        if (!screenTop) {
            continue;
        }

        // draw the head bar border
        Vec2 barLeftTop = {
                screenTop->x - Settings::Esp::barWidth / 2,
                screenTop->y - (Settings::Esp::barHeight + Settings::Esp::barMoveUp)
        };
        Vec2 barRightBottom = {
                screenTop->x + Settings::Esp::barWidth / 2,
                screenTop->y - Settings::Esp::barMoveUp
        };

        auto barBorderColor = player->getTeamId() == localPlayer->getTeamId() ?
                              Settings::Esp::teammateColor : Settings::Esp::enemyColor;

        ImGui::GetForegroundDrawList()->AddRect(
                ImVec2(barLeftTop.x, barLeftTop.y),
                ImVec2(barRightBottom.x, barRightBottom.y),
                barBorderColor
        );

        // draw name health bar
        Vec2 upperAreaLeftTop = {
                barLeftTop.x + 1,
                barLeftTop.y + 1
        };
        Vec2 upperAreaRightBottom = {
                barLeftTop.x + Settings::Esp::barWidth - 1,
                barLeftTop.y + Settings::Esp::barHeight * 0.6f - 1
        };
        Vec2 lowerAreaLeftTop = {
                barLeftTop.x + 1,
                barLeftTop.y + Settings::Esp::barHeight * 0.6f
        };
        Vec2 lowerAreaRightBottom = {
                barRightBottom.x - 1,
                barRightBottom.y - 1
        };

        ImGui::GetForegroundDrawList()->AddRectFilled(
                ImVec2(upperAreaLeftTop.x, upperAreaLeftTop.y),
                ImVec2(upperAreaRightBottom.x, upperAreaRightBottom.y),
                Settings::Esp::nameBackgroundColor
        );

        ImGui::GetForegroundDrawList()->AddText(
                ImVec2(upperAreaLeftTop.x + 1, upperAreaLeftTop.y + 1),
                ImColor(255,255,255),
                player->getName().c_str()
        );

        ImGui::GetForegroundDrawList()->AddRectFilled(
                ImVec2(lowerAreaLeftTop.x, lowerAreaLeftTop.y),
                ImVec2(lowerAreaRightBottom.x, lowerAreaRightBottom.y),
                Settings::Esp::healthBackgroundColor
        );

        Vec2 lowerInnerLeftTop = {
                lowerAreaLeftTop.x + 1,
                lowerAreaLeftTop.y + 1
        };
        Vec2 lowerInnerRightBottom = {
                lowerAreaRightBottom.x - 1,
                lowerAreaRightBottom.y - 1
        };

        float health = player->getHealth();
        float fullHealthLength = lowerInnerRightBottom.x - lowerInnerLeftTop.x;
        float healthLength = fullHealthLength * health / 100.f;
        Vec2 healthLeftTop = {
                lowerInnerLeftTop.x,
                lowerInnerLeftTop.y
        };
        Vec2 healthRightBottom = {
                lowerInnerLeftTop.x + healthLength,
                lowerInnerRightBottom.y
        };
        ImColor healthBarColor;
        if (health >= 50) {
            healthBarColor = Settings::Esp::healthFullColor;
        } else if (health >= 20) {
            healthBarColor = Settings::Esp::healthHalfColor;
        } else {
            healthBarColor = Settings::Esp::healthLowColor;
        }
        ImGui::GetForegroundDrawList()->AddRectFilled(
                ImVec2(healthLeftTop.x, healthLeftTop.y),
                ImVec2(healthRightBottom.x, healthRightBottom.y),
                healthBarColor
        );
    }
}
