#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "mem/memory_accessor.h"
#include "decrypt.h"
#include "name_pool.h"
#include "helper.h"
#include "offset.h"
#include "structure.h"
#include "player.h"
#include "log.h"
#include "game.h"

#include "world_to_screen/world_to_screen.h"

std::shared_ptr<PlayerInterface> Game::getLocalPlayer() {

    uint64_t playerController = PUBG::getPlayerController();

    uint64_t acknowledgedPawn = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            playerController + Offset_AcknowledgedPawn
    ));

    return std::make_shared<LocalPlayer>(acknowledgedPawn);
}

void showItem(uint64_t actor) {
    uint64_t rootComponent = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            actor + Offset_RootComponent
    ));

    // ComponentLocation = ComponentToWorld.Translation
    glm::vec3 position = MemoryAccessor<glm::vec3>(
            rootComponent + Offset_ComponentLocation
    );
    std::optional<glm::vec2> screenPos = WorldToScreen::translate(position);
    if (!screenPos) {
        return;
    }

    uint32_t actorNameId = PUBG::decryptId(MemoryAccessor<uint32_t>(
            actor + Offset_ObjID
    ));
    std::string actorName = PUBG::getName(actorNameId);

    if (actorName == "DroppedItem") {
        uint64_t item = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
                actor + Offset_DroppedItem_Item
        ));
        uint64_t itemTable = MemoryAccessor<uint64_t>(
                item + Offset_ItemTable
        );
        uint32_t itemNameId = MemoryAccessor<uint32_t>(
                itemTable + Offset_ItemID
        );
        actorName = PUBG::getName(itemNameId);

        Module::view->drawCircle({screenPos->x, screenPos->y},
                                 10.0f, ImColor(255, 0, 0));
        Module::view->drawString({screenPos->x, screenPos->y + 10.f},
                                 ImColor(255, 0, 0), actorName);
    }
}

std::vector<std::shared_ptr<PlayerInterface>> Game::getPlayers() {

    std::vector<std::shared_ptr<PlayerInterface>> players;

    uint64_t gworld = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            "TslGame.exe",
            Offset_GWorld
    ));

    uint64_t currentLevel = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            gworld + Offset_CurrentLevel
    ));

    uint64_t pActors = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            currentLevel + Offset_Actors
    ));

    TArray actors = MemoryAccessor<TArray>(
            pActors
    );

    for (uint32_t i = 0; i < actors.cnt; i++) {
        uint64_t actor = MemoryAccessor<uint64_t>(
                actors.ptr + i * sizeof(uint64_t)
        );
        uint32_t actorNameId = PUBG::decryptId(MemoryAccessor<uint32_t>(
                actor + Offset_ObjID
        ));
        std::string actorName = PUBG::getName(actorNameId);

//        showItem(actor);
//
        if (PUBG::isPlayer(actorName)) {
            players.emplace_back(std::make_shared<Player>(actor));
        }
    }

    return players;
}

glm::mat4 Game::getVPMatrix() {
    CameraInfo camera = PUBG::getCameraInfo();

    glm::mat4 view = getViewMatrix(camera);
    glm::mat4 projection = getProjectionMatrix(camera);

    return projection * view;
}

glm::vec3 Game::viewAngleToOrientation(glm::vec3 viewAngle) {
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    float alpha = glm::radians(viewAngle.y);
    float beta = glm::radians(viewAngle.x);
    glm::vec3 vXY = glm::normalize(glm::rotate(X, alpha, Z));
    glm::vec3 vCross = glm::normalize(glm::cross(vXY, Z));
    glm::vec3 vXYZ = glm::normalize(glm::rotate(vXY, beta, vCross));
    return vXYZ;
}

glm::vec3 Game::orientationToViewAngle(glm::vec3 orientation) {
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vXY = glm::normalize(glm::vec3(orientation.x, orientation.y, 0.0f));
    glm::vec3 vCross = glm::normalize(glm::cross(vXY, Z));
    glm::vec3 vXYZ = glm::normalize(orientation);
    float alpha = glm::orientedAngle(X, vXY, Z);
    float beta = glm::orientedAngle(vXY, vXYZ, vCross);
    alpha = glm::clamp(glm::degrees(alpha), -180.0f, 180.0f);
    beta = glm::clamp(glm::degrees(beta), -75.0f, 75.0f);
    return {beta, alpha, 0.0f};
}

// copied from UE4 FInverseRotationMatrix
glm::mat4 Game::getInverseRotationMatrix(FRotator rotation) {

    float SY = glm::sin(glm::radians(rotation.yaw));
    float CY = glm::cos(glm::radians(rotation.yaw));
    float SP = glm::sin(glm::radians(rotation.pitch));
    float CP = glm::cos(glm::radians(rotation.pitch));
    float SR = glm::sin(glm::radians(rotation.roll));
    float CR = glm::cos(glm::radians(rotation.roll));

    glm::mat4 yaw = glm::mat4(
            +CY, -SY, 0.0f, 0.0f,
            +SY, +CY, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 pitch = glm::mat4(
            +CP, 0.0f, -SP, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            +SP, 0.0f, +CP, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 roll = glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, +CR, +SR, 0.0f,
            0.0f, -SR, +CR, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    );

    return roll * pitch * yaw;
}

// copied from UE4 UpdateViewMatrix
glm::mat4 Game::getViewMatrix(const CameraInfo &camera) {
    glm::mat4 viewPlanes = {
            0, 0, 1, 0,
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 1,
    };

    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -camera.location);
    viewMatrix = getInverseRotationMatrix(camera.rotation) * viewMatrix;
    viewMatrix = viewPlanes * viewMatrix;
    return viewMatrix;
}

glm::mat4 Game::getProjectionMatrix(const CameraInfo &camera) {
    glm::vec2 windowSize = Module::view->getSize();
    float aspect = windowSize.x / windowSize.y;
    float fovy = 2 * atan(tan(glm::radians(camera.FOV) / 2) / aspect);
    glm::mat4 projection = glm::infinitePerspective(fovy, aspect, 0.1f);
    return projection;
}