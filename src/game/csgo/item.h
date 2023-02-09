#ifndef EVIL_CAT_GAME_CSGO_ITEM_H
#define EVIL_CAT_GAME_CSGO_ITEM_H

#include "game/item_interface.h"

class Item : public ItemInterface {
public:
    Item(uint32_t ptr, std::string name);

    glm::vec3 getPosition() override;

    std::string getName() override;

    std::string getExtra() override;

    int getLevel() override;

protected:
    uint32_t _this;
    std::string name;
};

class PlantedC4 : public Item {
public:
    using Item::Item;

    std::string getExtra() override;
};

#endif //EVIL_CAT_GAME_CSGO_ITEM_H
