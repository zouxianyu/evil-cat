#include <memory>
#include <optional>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "module.h"
#include "radar.h"

namespace Settings::Radar {
    bool on = true;
    glm::vec2 normalizedCenter = {0.5f, 0.7f};
    float radius = 80.f;
    float scale = 1.0f;
    bool rotate = true;
    float playerRadius = 6.f;
    float viewLineScale = 15.f;
    ImColor backgroundColor = ImColor(0, 0, 0, 130);
    ImColor borderColor = ImColor(255, 255, 255, 255);
    ImColor teammateColor = ImColor(0, 255, 0, 255);
    ImColor enemyColor = ImColor(255, 0, 0, 255);
}

void Radar::callback() {
    if (!Settings::Radar::on) {
        return;
    }
    std::shared_ptr<PlayerInterface> localPlayer = Module::game->getLocalPlayer();
    std::vector<std::shared_ptr<PlayerInterface>> players = Module::game->getPlayers();

    glm::vec3 localPosition = localPlayer->getPosition();

    glm::vec3 defaultViewAngle = {0.f, 0.f, 0.f};
    glm::vec3 defaultOrientation = Module::game->viewAngleToOrientation(defaultViewAngle);

    glm::vec2 defaultOrientation2D = glm::normalize(
            glm::vec2{defaultOrientation.x, defaultOrientation.y}
    );

    glm::vec3 currentViewAngle = localPlayer->getViewAngle();
    glm::vec3 currentOrientation = Module::game->viewAngleToOrientation(currentViewAngle);

    glm::vec2 currentOrientation2D = glm::normalize(
            glm::vec2{currentOrientation.x, currentOrientation.y}
    );

    float alpha = glm::orientedAngle(currentOrientation2D, defaultOrientation2D);

    if (Settings::Radar::rotate) {
        currentOrientation2D = glm::rotate(currentOrientation2D, alpha);
    }

    // calculate the radar's position
    glm::vec2 windowSize = Module::game->getWindowSize();

    glm::vec2 radarPosition = Settings::Radar::normalizedCenter * windowSize;

    // draw background and border
    ImGui::GetBackgroundDrawList()->AddCircleFilled(
            ImVec2(radarPosition.x, radarPosition.y),
            Settings::Radar::radius,
            Settings::Radar::backgroundColor
    );
    ImGui::GetBackgroundDrawList()->AddCircle(
            ImVec2(radarPosition.x, radarPosition.y),
            Settings::Radar::radius,
            Settings::Radar::borderColor,
            0,
            2.f
    );

    // draw local player
    // local player is always at the center of the radar

    ImVec2 drawingCenter = ImVec2(radarPosition.x, radarPosition.y);
    glm::vec2 vDrawingEnd =
            radarPosition + Settings::Radar::viewLineScale * currentOrientation2D;
    ImVec2 drawingEnd = ImVec2(vDrawingEnd.x, vDrawingEnd.y);

    ImGui::GetBackgroundDrawList()->AddCircleFilled(
            drawingCenter,
            Settings::Radar::playerRadius,
            Settings::Radar::teammateColor
    );
    ImGui::GetBackgroundDrawList()->AddLine(
            drawingCenter,
            drawingEnd,
            Settings::Radar::teammateColor,
            2.f
    );

    for (const auto &player: players) {

        // skip local player and dead players
        if (*localPlayer == *player || player->getHealth() <= 0) {
            continue;
        }

        // choose player color showing on radar
        ImColor playerColor = player->getTeamId() == localPlayer->getTeamId() ?
                              Settings::Radar::teammateColor : Settings::Radar::enemyColor;

        // calculate player 2D position
        glm::vec3 playerPosition = player->getPosition();

        glm::vec2 playerPosition2D = {
                playerPosition.x - localPosition.x,
                playerPosition.y - localPosition.y
        };

        // apply the scale to the player 2d position
        playerPosition2D *= Settings::Radar::scale;

        // if the player can't display inside the radar, then skip it
        if (glm::distance(glm::vec2(0.f, 0.f), playerPosition2D) >=
            Settings::Radar::radius) {
            continue;
        }

        // rotate it to a proper angle
        if (Settings::Radar::rotate) {
            playerPosition2D = glm::rotate(playerPosition2D, alpha);
        }

        glm::vec3 playerViewAngle = player->getViewAngle();

        glm::vec3 playerOrientation = Module::game->viewAngleToOrientation(playerViewAngle);

        glm::vec2 playerOrientation2D = glm::normalize(
                glm::vec2{playerOrientation.x, playerOrientation.y}
        );

        if (Settings::Radar::rotate) {
            playerOrientation2D = glm::rotate(playerOrientation2D, alpha);
        }

        playerPosition2D += radarPosition;

        // now we have the player position and orientation

        drawingCenter = ImVec2(playerPosition2D.x, playerPosition2D.y);
        vDrawingEnd = playerPosition2D + Settings::Radar::viewLineScale * playerOrientation2D;
        drawingEnd = ImVec2(vDrawingEnd.x, vDrawingEnd.y);

        ImGui::GetBackgroundDrawList()->AddCircleFilled(
                drawingCenter,
                Settings::Radar::playerRadius,
                playerColor
        );
        ImGui::GetBackgroundDrawList()->AddLine(
                drawingCenter,
                drawingEnd,
                playerColor,
                2.f
        );

    }
}
