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
    return std::make_shared<Player>(
            MemoryAccessor<gameptr_t>{
                    "engine.dll", hazedumper::signatures::dwClientState,
                    {hazedumper::signatures::dwClientState_GetLocalPlayer}
            }
    );
}

inline static bool isValidPlayer(gameptr_t entity) {
    return MemoryAccessor<uint8_t>{entity + hazedumper::signatures::m_bDormant} != 0;
}

std::vector<std::shared_ptr<PlayerInterface>> Game::getPlayers() {
    std::vector<std::shared_ptr<PlayerInterface>> players;

    // get player list address
    gameptr_t playerList = MemoryAccessor<gameptr_t>{
            "client.dll",
            hazedumper::signatures::dwEntityList
    };

    // define player entry
    struct PlayerEntry {
        gameptr_t player;
        uint32_t fill;
        gameptr_t next;
        gameptr_t prev;
    };

    // rec info
    gameptr_t head = playerList;
    gameptr_t curr = head;

    // get all players
    PlayerEntry entry;
    do {
        entry = MemoryAccessor<PlayerEntry>{curr};
        if (entry.player && isValidPlayer(entry.player)) {
            players.emplace_back(std::make_shared<Player>(entry.player));
        }
        curr = entry.next;
    } while (entry.next != head);

    return players;
}

glm::mat4 Game::getVPMatrix() {
    return MemoryAccessor<glm::mat4>{
            "client.dll",
            hazedumper::signatures::dwViewMatrix
    };
}

glm::vec2 Game::getWindowSize() {
    return {1600, 900};
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
    glm::vec3 localPlayerPos = getLocalPlayer()->getPosition();
    glm::vec3 playerPos = player->getPosition();
    return glm::distance(localPlayerPos, playerPos) * 0.3f;
}