#ifndef EVIL_CAT_GAME_ENTITY_CONTAINER_H
#define EVIL_CAT_GAME_ENTITY_CONTAINER_H

#include <memory>
#include <vector>
#include "player_interface.h"
#include "item_interface.h"

struct EntityContainer {
    std::shared_ptr<PlayerInterface> localPlayer;
    std::vector<std::shared_ptr<PlayerInterface>> players;
    std::vector<std::shared_ptr<ItemInterface>> items;
};

#endif //EVIL_CAT_GAME_ENTITY_CONTAINER_H
