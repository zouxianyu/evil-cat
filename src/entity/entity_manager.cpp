#include <string>
#include <iostream>
#include "mem/process_memory_accessor.h"
#include "entity_manager.h"


EntityManager::EntityManager() = default;

EntityManager &EntityManager::getInstance() {
    static EntityManager instance;
    return instance;
}

Player EntityManager::getLocalPlayer() {
    return Player{ProcessMemoryAccessor<void *>{"ac_client.exe", offset::localPlayer}};
}

glm::mat4 EntityManager::getViewProjectionMatrix() {

    Mat4 vpMatrix = ProcessMemoryAccessor<Mat4>{"ac_client.exe", offset::vpMatrix};

    return {vpMatrix.m[0][0], vpMatrix.m[0][1], vpMatrix.m[0][2], vpMatrix.m[0][3],
            vpMatrix.m[1][0], vpMatrix.m[1][1], vpMatrix.m[1][2], vpMatrix.m[1][3],
            vpMatrix.m[2][0], vpMatrix.m[2][1], vpMatrix.m[2][2], vpMatrix.m[2][3],
            vpMatrix.m[3][0], vpMatrix.m[3][1], vpMatrix.m[3][2], vpMatrix.m[3][3]};
}

std::vector<Player> EntityManager::getPlayers() {
    std::vector<Player> players;
    uintptr_t playerList = ProcessMemoryAccessor<uintptr_t>{"ac_client.exe", offset::playerList};
    int n = ProcessMemoryAccessor<int>{"ac_client.exe", offset::playerCount};
    uintptr_t self = ProcessMemoryAccessor<uintptr_t>{"ac_client.exe", offset::localPlayer};
    for (int i = 0; i < n; i++) {
        uintptr_t playerAddr = ProcessMemoryAccessor{reinterpret_cast<uintptr_t *>(playerList + i * sizeof(uintptr_t))};
        if (playerAddr == 0 || playerAddr == self) {
            continue;
        }
        players.emplace_back(Player{reinterpret_cast<void *>(playerAddr)});
    }
    return players;
}




