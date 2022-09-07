#ifndef EVIL_CAT_GAME_INTERFACE_PLAYER_BASIC_INTERFACE_H
#define EVIL_CAT_GAME_INTERFACE_PLAYER_BASIC_INTERFACE_H

#include <string>
#include "type.h"

// this interface contains basic information about a player
// use this interface to get basic information about a player in the core of the framework
// so you can draw basic esp on the screen
class PlayerBasicInterface {
public:
    // get the position of the player
    // we assume the position represents the bottom center of the player
    virtual Vec3 getPosition() = 0;

    // get the height of the player,
    // so we can draw a box with proper size around the player
    // usually, a constant is sufficient
    virtual float getHeight() = 0;

    // the camera position is the camera position of a player
    // we can use the position combined with the view angle
    // to calculate the orientation of the player and draw a view line
    virtual Vec3 getCameraPosition() = 0;

    // get the view angle of the player
    // we can use the view angle to calculate the orientation of the player
    // 3D esp needs the orientation of the player
    virtual Vec3 getViewAngle() = 0;

    // set the view angle of the player
    // aimbot needs to set the view angle of the local player
    virtual void setViewAngle(Vec3 angle) = 0;

    // get the name of the player
    // we can draw it over the esp box
    virtual std::string getName() = 0;

    // the team id represents the team of the player
    // if the given player has the same team id as the local player
    // we think the player is a teammate, otherwise, the player is an enemy
    virtual int getTeamId() = 0;

    // get the health of the player
    // show health bar over the player when the ESP is on
    virtual float getHealth() = 0;

    // we call use the compare operator to identify whether the player is the local player
    virtual bool operator==(const PlayerBasicInterface &other) const = 0;
};

#endif //EVIL_CAT_GAME_INTERFACE_PLAYER_BASIC_INTERFACE_H
