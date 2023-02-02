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

std::string Radar::getName() {
    return "radar";
}

void Radar::menuCallback() {
    ImGui::Checkbox("radar", &Settings::Radar::on);
    ImGui::Checkbox("rotate", &Settings::Radar::rotate);
    ImGui::SliderFloat("radius", &Settings::Radar::radius, 0.f, 100.f);
    ImGui::SliderFloat("scale", &Settings::Radar::scale, 0.f, 10.f);
    ImGui::Separator();
    ImGui::ColorEdit4("background color", (float *)&Settings::Radar::backgroundColor.Value);
    ImGui::ColorEdit4("border color", (float *)&Settings::Radar::borderColor.Value);
    ImGui::ColorEdit4("teammate color", (float *)&Settings::Radar::teammateColor.Value);
    ImGui::ColorEdit4("enemy color", (float *) &Settings::Radar::enemyColor.Value);
}

inline static glm::vec2 rightHandleFix(glm::vec2 in) {
#ifndef GLM_LEFT_HANDED
    return {-in.x, in.y};
#else
    return in;
#endif
}

void Radar::serviceCallback() {
    if (!Settings::Radar::on) {
        return;
    }
    EntityContainer container = Module::game->getEntities();
    std::shared_ptr<PlayerInterface> &localPlayer = container.localPlayer;
    std::vector<std::shared_ptr<PlayerInterface>> &players = container.players;

    glm::vec3 localPosition = localPlayer->getPosition();

    glm::vec2 defaultOrientation2D = {0.f, -1.f};

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
    glm::vec2 windowSize = Module::view->getSize();

    glm::vec2 radarPosition = Settings::Radar::normalizedCenter * windowSize;

    // draw background and border
    Module::view->drawCircleFilled(
            ImVec2(radarPosition.x, radarPosition.y),
            Settings::Radar::radius,
            Settings::Radar::backgroundColor
    );
    Module::view->drawCircle(
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
            radarPosition + Settings::Radar::viewLineScale * rightHandleFix(currentOrientation2D);
    ImVec2 drawingEnd = ImVec2(vDrawingEnd.x, vDrawingEnd.y);

    Module::view->drawCircleFilled(
            drawingCenter,
            Settings::Radar::playerRadius,
            Settings::Radar::teammateColor
    );
    Module::view->drawLine(
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
        playerPosition2D = rightHandleFix(playerPosition2D);

        glm::vec3 playerViewAngle = player->getViewAngle();

        glm::vec3 playerOrientation = Module::game->viewAngleToOrientation(playerViewAngle);

        glm::vec2 playerOrientation2D = glm::normalize(
                glm::vec2{playerOrientation.x, playerOrientation.y}
        );

        if (Settings::Radar::rotate) {
            playerOrientation2D = glm::rotate(playerOrientation2D, alpha);
        }
        playerOrientation2D = rightHandleFix(playerOrientation2D);

        playerPosition2D += radarPosition;

        // now we have the player position and orientation

        drawingCenter = ImVec2(playerPosition2D.x, playerPosition2D.y);
        vDrawingEnd = playerPosition2D + Settings::Radar::viewLineScale * playerOrientation2D;
        drawingEnd = ImVec2(vDrawingEnd.x, vDrawingEnd.y);

        Module::view->drawCircleFilled(
                drawingCenter,
                Settings::Radar::playerRadius,
                playerColor
        );
        Module::view->drawLine(
                drawingCenter,
                drawingEnd,
                playerColor,
                2.f
        );

    }
}
