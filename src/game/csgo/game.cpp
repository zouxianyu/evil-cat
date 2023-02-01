#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "player.h"
#include "offset.h"
#include "mem/memory_accessor.h"
#include "game.h"

std::shared_ptr<PlayerInterface> Game::getLocalPlayer() {
    return std::make_shared<LocalPlayer>(
            MemoryAccessor<gameptr_t>{
                    "client.dll", hazedumper::signatures::dwLocalPlayer,
            }
    );
}

inline static bool isValidPlayer(gameptr_t entity) {
    return MemoryAccessor<uint8_t>{entity + hazedumper::signatures::m_bDormant} != 0;
}

std::vector<std::shared_ptr<PlayerInterface>> Game::getPlayers() {
    std::vector<std::shared_ptr<PlayerInterface>> players;

    // get max player count
    int playerMaxCount = MemoryAccessor<int>{
            "engine.dll", hazedumper::signatures::dwClientState,
            {hazedumper::signatures::dwClientState_MaxPlayer}
    };

    // define player entry
    struct PlayerEntry {
        gameptr_t player;
        uint32_t fill;
        gameptr_t next;
        gameptr_t prev;
    };

    auto entries = std::make_unique<PlayerEntry[]>(playerMaxCount);
    MemoryAccessor<PlayerEntry>{
            "client.dll",hazedumper::signatures::dwEntityList
    }.get(entries.get(), playerMaxCount);

    for (int i = 0; i < playerMaxCount; i++) {
        auto &entry = entries[i];
        if (entry.player) {
            players.emplace_back(std::make_shared<Player>(entry.player));
        }
    }

    return players;
}

glm::mat4 Game::getVPMatrix() {
    glm::mat4 mat = MemoryAccessor<glm::mat4>{
            "client.dll",
            hazedumper::signatures::dwViewMatrix
    };
    return glm::transpose(mat);
}

glm::vec3 Game::viewAngleToOrientation(glm::vec3 viewAngle) {
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    float alpha = glm::radians(viewAngle.y);
    float beta = -glm::radians(viewAngle.x);
    glm::vec3 vXY = glm::normalize(glm::rotate(X, alpha, Z));
    glm::vec3 vCross = glm::normalize(glm::cross(vXY,Z));
    glm::vec3 vXYZ = glm::normalize(glm::rotate(vXY, beta, vCross));
    return vXYZ;
}

glm::vec3 Game::orientationToViewAngle(glm::vec3 orientation) {
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 vXY = glm::normalize(glm::vec3(orientation.x, orientation.y, 0.0f));
    glm::vec3 vXYZ = glm::normalize(orientation);
    float alpha = glm::orientedAngle(X, vXY, Z);
    float beta = glm::angle(vXYZ, Z) - glm::pi<float>() / 2.0f;
    alpha = glm::clamp(glm::degrees(alpha), -180.0f, 180.0f);
    beta = glm::clamp(glm::degrees(beta), -89.0f, 89.0f);
    return {beta, alpha, 0.0f};
}
