#include <vector>
#include <memory>
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
        players.emplace_back(std::make_shared<Player>(
            reinterpret_cast<void *>(playerAddr)
        ));
    }
    return players;
}

std::shared_ptr<Mat4> Game::getVPMatrix() {
    return std::make_shared<Mat4>(
        ProcessMemoryAccessor<Mat4>{
            "ac_client.exe",
            Offset::vpMatrix
        }
    );
}
