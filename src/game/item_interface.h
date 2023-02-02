#ifndef EVIL_CAT_GAME_ITEM_INTERFACE_H
#define EVIL_CAT_GAME_ITEM_INTERFACE_H

#include <string>
#include <glm/glm.hpp>

class ItemInterface {
public:
    virtual glm::vec3 getPosition() = 0;

    virtual std::string getName() = 0;

    virtual std::string getExtra() = 0;

    virtual int getLevel() = 0;

    virtual ~ItemInterface() = default;
};

#endif //EVIL_CAT_GAME_ITEM_INTERFACE_H
