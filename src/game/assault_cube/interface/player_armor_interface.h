#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_ARMOR_INTERFACE_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_ARMOR_INTERFACE_H

class PlayerArmorInterface {
public:
    virtual float getArmor() = 0;

    virtual void setArmor(float armor) = 0;
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_ARMOR_INTERFACE_H
