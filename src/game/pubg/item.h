#ifndef EVIL_CAT_GAME_PUBG_ITEM_H
#define EVIL_CAT_GAME_PUBG_ITEM_H

#include "game/item_interface.h"
#include "structure.h"

class Item : public ItemInterface {
public:
    Item(gameptr_t address, ItemInfo info);

    glm::vec3 getPosition() override;

    std::string getName() override;

    std::string getExtra() override;

    int getLevel() override;

protected:
    gameptr_t _this;
    ItemInfo info;
};

#endif //EVIL_CAT_GAME_PUBG_ITEM_H
