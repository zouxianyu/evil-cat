#include <vector>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "player.h"
#include "offset/offset.h"
#include "mem/process_memory_accessor.h"
#include "game.h"

Game &Game::getInstance() {
    static Game instance;
    return instance;
}

std::shared_ptr<PlayerBasicInterface> Game::getLocalPlayer() {
    return std::make_shared<Player>(
        ProcessMemoryAccessor<void *>{
            "ac_client.exe",
            Offset::localPlayer
        }
    );
}

std::vector<std::shared_ptr<PlayerBasicInterface>> Game::getPlayers() {
    std::vector<std::shared_ptr<PlayerBasicInterface>> players;

    // get player list address
    uintptr_t playerList = ProcessMemoryAccessor<uintptr_t>{
        "ac_client.exe",
        Offset::playerList
    };

    // get player count
    int n = ProcessMemoryAccessor<int>{
        "ac_client.exe",
        Offset::playerCount
    };

    // construct all player wrappers and put them into the vector
    for (int i = 0; i < n; i++) {
        uintptr_t playerAddr = ProcessMemoryAccessor<uintptr_t>{
            reinterpret_cast<uintptr_t *>(playerList + i * sizeof(uintptr_t))
        };
        if (!playerAddr) {
            continue;
        }
        players.emplace_back(std::make_shared<Player>(
            reinterpret_cast<void *>(playerAddr)
        ));
    }
    return players;
}

std::shared_ptr<glm::mat4> Game::getVPMatrix() {
    return std::make_shared<glm::mat4>(
        ProcessMemoryAccessor<glm::mat4>{
            "ac_client.exe",
            Offset::vpMatrix
        }
    );
}

glm::vec2 Game::getWindowSize() {
    return glm::vec2{1024, 768};
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
