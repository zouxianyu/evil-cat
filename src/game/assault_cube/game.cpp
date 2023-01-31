#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "player.h"
#include "offset/offset.h"
#include "mem/memory_accessor.h"
#include "game.h"

std::shared_ptr<PlayerInterface> Game::getLocalPlayer() {
    return std::make_shared<Player>(
            MemoryAccessor<gameptr_t>{
            "ac_client.exe",
            Offset::localPlayer
        }
    );
}

std::vector<std::shared_ptr<PlayerInterface>> Game::getPlayers() {
    std::vector<std::shared_ptr<PlayerInterface>> players;

    // get player list address
    gameptr_t playerList = MemoryAccessor<gameptr_t>{
        "ac_client.exe",
        Offset::playerList
    };

    // get player count
    int n = MemoryAccessor<int>{
        "ac_client.exe",
        Offset::playerCount
    };

    // construct all player wrappers and put them into the vector
    for (int i = 0; i < n; i++) {
        gameptr_t playerAddr = MemoryAccessor<gameptr_t>{
            playerList + i * sizeof(gameptr_t)
        };
        if (!playerAddr) {
            continue;
        }
        players.emplace_back(std::make_shared<Player>(playerAddr));
    }
    return players;
}

glm::mat4 Game::getVPMatrix() {
    return MemoryAccessor<glm::mat4>{
        "ac_client.exe",
        Offset::vpMatrix
    };
}

glm::vec2 Game::getWindowSize() {
    glm::vec<2, int> windowSize = MemoryAccessor<glm::vec<2, int>>{
            "ac_client.exe",
            Offset::windowSize
    };
    return glm::vec2{windowSize[0], windowSize[1]};
}

glm::vec3 Game::viewAngleToOrientation(glm::vec3 viewAngle) {
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 negtiveY = glm::vec3(0.0f, -1.0f, 0.0f);
    float alpha = viewAngle.x * glm::pi<float>() / 180.0f;
    float beta = viewAngle.y * glm::pi<float>() / 180.0f;
    glm::vec3 vecXY = glm::rotate(negtiveY, alpha, Z);
    glm::vec3 axis = glm::normalize(glm::cross(vecXY, Z));
    glm::vec3 vecXYZ = glm::normalize(glm::rotate(vecXY, beta, axis));
    return vecXYZ;
}

glm::vec3 Game::orientationToViewAngle(glm::vec3 orientation) {
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 negtiveY = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 orientationXY = glm::normalize(
            glm::vec3(orientation.x, orientation.y, 0.f)
    );
    float alpha = glm::orientedAngle(negtiveY, orientationXY, Z);
    glm::vec3 orientationXYZ = glm::normalize(orientation);
    float beta = glm::pi<float>() / 2.0f - glm::angle(Z, orientationXYZ);
    return {alpha * 180.0f / glm::pi<float>(), beta * 180.0f / glm::pi<float>(), 0.0f};
}

float Game::getDistance(std::shared_ptr<PlayerInterface> player) {
    // TODO: remove getDistance function, use scaling factor instead
    glm::vec3 localPlayerPos = Module::game->getLocalPlayer()->getPosition();
    glm::vec3 playerPos = player->getPosition();
    return glm::distance(localPlayerPos, playerPos) * 0.3f;
}