#include <iostream>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "game.h"
#include "game/interface/player_basic_interface.h"
#include "service/world_to_screen/world_to_screen.h"
#include "type.h"
#include "settings.h"
#include "esp.h"

namespace Settings::Esp {
    bool on = true;
    BoxType boxType = BoxType::show2D;
    bool showViewLine = true;
    bool showHeadBar = true;
    bool showHeadDot = true;

    ImColor teammateColor = ImColor(0, 255, 0);
    ImColor enemyColor = ImColor(255, 0, 0);
    ImColor viewLineColor = ImColor(255, 255, 255);
    ImColor healthColor = ImColor(0, 255, 0);
    ImColor armorColor = ImColor(0, 0, 255);
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
//        case Settings::Esp::BoxType::show3D:
//            showEsp3D(localPlayer, players);
//            break;
    }

//    // show view line
//    if (Settings::Esp::showViewLine) {
//        showViewLine(localPlayer, players);
//    }
//
//    // show head bar
//    if (Settings::Esp::showHeadBar) {
//        showHeadBar(localPlayer, players);
//    }
//
//    // show head dot
//    if (Settings::Esp::showHeadDot) {
//        showHeadDot(localPlayer, players);
//    }

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

//void Esp::showEspBox3D(Player &localPlayer, std::vector<Player> &players) {
//    for (int i = 0; i < players.size(); i++) {
//        Player &player = players[i];
//        if (player.getHealth() <= 0) {
//            continue;
//        }
//
//        Vec3 head = player.getHead();
//        Vec3 feet = player.getFeet();
//        Vec3 viewAngle = player.getViewAngle();
//        Vec3 orientation = viewAngleToOrientation(viewAngle);
//        Vec3 viewLineEnd = head + orientation;
//        auto boxColor = player.getTeam() == localPlayer.getTeam() ?
//                        Settings::getInstance().friendlyColor : Settings::getInstance().enemyColor;
//        auto viewLineColor = Settings::getInstance().viewLineColor;
//
//        Vec2 screenHead{};
//        if (!WorldToScreen::getInstance().translate(head, screenHead)) {
//            continue;
//        }
//        Vec2 screenFeet{};
//        if (!WorldToScreen::getInstance().translate(feet, screenFeet)) {
//            continue;
//        }
//
//        // show box 3d
//        float height = head.z - feet.z;
//        float width = height / 2;
//        glm::vec4 corners[8] = {
//                {width / 2,  width / 2,  0,      1},
//                {width / 2,  -width / 2, 0,      1},
//                {-width / 2, -width / 2, 0,      1},
//                {-width / 2, width / 2,  0,      1},
//                {width / 2,  width / 2,  height, 1},
//                {width / 2,  -width / 2, height, 1},
//                {-width / 2, -width / 2, height, 1},
//                {-width / 2, width / 2,  height, 1},
//        };
//        Vec2 screenCorners[8] = {};
//
//        glm::vec3 orientation2d = glm::normalize(glm::vec3(orientation.x, orientation.y, 0));
//        glm::mat4 esp3dRotation = glm::rotate(
//                glm::mat4(1.0f),
//                glm::angle(orientation2d, glm::vec3(1.f, 0.f, 0.f)),
//                glm::vec3(0.0f, 0.0f, 1.0f)
//        );
//        glm::mat4 esp3dTranslate = glm::translate(glm::mat4(1.0f),
//                                                  glm::vec3(feet.x, feet.y, feet.z));
//        for (auto &v: corners) {
//            v = esp3dTranslate * esp3dRotation * v;
//            v /= v.w;
//        }
//
//        // translate to screen coordination
//        bool notShow = false;
//        for (int j = 0; j < 8; j++) {
//            if (!WorldToScreen::getInstance().translate(
//                    Vec3{corners[j].x, corners[j].y, corners[j].z},
//                    screenCorners[j])) {
//                notShow = true;
//                break;
//            }
//        }
//        if (notShow) {
//            continue;
//        }
//
//        //show box 3d
//        for (int j: {0, 1, 2, 3}) {
//            ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenCorners[j].x, screenCorners[j].y),
//                                                    ImVec2(screenCorners[(j + 1) % 4].x,
//                                                           screenCorners[(j + 1) % 4].y),
//                                                    boxColor);
//            ImGui::GetForegroundDrawList()->AddLine(
//                    ImVec2(screenCorners[4 + j].x, screenCorners[4 + j].y),
//                    ImVec2(screenCorners[4 + (j + 1) % 4].x,
//                           screenCorners[4 + (j + 1) % 4].y),
//                    boxColor);
//            ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenCorners[j].x, screenCorners[j].y),
//                                                    ImVec2(screenCorners[j + 4].x,
//                                                           screenCorners[j + 4].y),
//                                                    boxColor);
//        }
//
//        // show view line
//        Vec2 screenViewLineEnd{};
//        if (!WorldToScreen::getInstance().translate(viewLineEnd, screenViewLineEnd)) {
//            continue;
//        }
//
//        ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenHead.x, screenHead.y),
//                                                ImVec2(screenViewLineEnd.x, screenViewLineEnd.y),
//                                                viewLineColor);
//    }
//}
//
//void Esp::showEspBox2D(Player &localPlayer, std::vector<Player> &players) {
//
//}
//
//void Esp::showHealthAndArmor(std::vector<Player> &players) {
//
//    for (int i = 0; i < players.size(); i++) {
//        Player &player = players[i];
//        if (player.getHealth() <= 0) {
//            continue;
//        }
//
//        Vec3 head = player.getHead();
//        Vec3 feet = player.getFeet();
//
//        // show box 2d
//        Vec2 screenHead{};
//        if (!WorldToScreen::getInstance().translate(head, screenHead)) {
//            continue;
//        }
//        Vec2 screenFeet{};
//        if (!WorldToScreen::getInstance().translate(feet, screenFeet)) {
//            continue;
//        }
//
//        float height = screenFeet.y - screenHead.y;
//        float width = height / 2;
//        float barWidth = 6.f;
//        float barSkip = 2.f;
//        float barThickness = 1.f;
//
//        int health = player.getHealth();
//        int armor = player.getArmor();
//        float healthBarHeight = health * (height - barThickness * 2) / 100.f;
//        float armorBarHeight = armor * (height - barThickness * 2) / 100.f;
//
//        ImGui::GetForegroundDrawList()->AddRectFilled(
//                ImVec2(screenFeet.x - width / 2 - barWidth - barSkip, screenFeet.y - height),
//                ImVec2(screenFeet.x - width / 2 - barSkip, screenFeet.y),
//                Settings::getInstance().backgroundColor);
//
//        ImGui::GetForegroundDrawList()->AddRectFilled(
//                ImVec2(screenFeet.x - width / 2 - barWidth + barThickness - barSkip,
//                       screenFeet.y + barThickness - healthBarHeight),
//                ImVec2(screenFeet.x - width / 2 - barThickness - barSkip,
//                       screenFeet.y - barThickness),
//                Settings::getInstance().healthColor);
//
//        if (armor != 0) {
//            ImGui::GetForegroundDrawList()->AddRectFilled(
//                    ImVec2(screenFeet.x + width / 2 + barSkip, screenFeet.y - height),
//                    ImVec2(screenFeet.x + width / 2 + barWidth + barSkip, screenFeet.y),
//                    Settings::getInstance().backgroundColor);
//
//            ImGui::GetForegroundDrawList()->AddRectFilled(
//                    ImVec2(screenFeet.x + width / 2 + barThickness + barSkip,
//                           screenFeet.y + barThickness - armorBarHeight),
//                    ImVec2(screenFeet.x + width / 2 + barWidth - barThickness +
//                           barSkip, screenFeet.y - barThickness),
//                    Settings::getInstance().armorColor);
//        }
//    }
//}
//
//
//
//
