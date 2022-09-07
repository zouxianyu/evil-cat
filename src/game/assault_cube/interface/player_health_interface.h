#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_HEALTH_INTERFACE_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_HEALTH_INTERFACE_H

class PlayerHealthInterface {
public:
    virtual float getHealth() = 0;

    virtual void setHealth(float health) = 0;
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_INTERFACE_PLAYER_HEALTH_INTERFACE_H
