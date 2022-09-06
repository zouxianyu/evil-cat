#include "mem/process_memory_accessor.h"
#include "offset/player.h"
#include "player.h"

Player::Player(void *address)
        : _this(address) {
}

Vec3 Player::getPosition() {
    return ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, feet)};
}

float Player::getHeight() {
    Vec3 feet = getPosition();
    Vec3 head = getCameraPosition();
    return head.z - feet.z;
}

Vec3 Player::getCameraPosition() {
    return ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, head)};
}

Vec3 Player::getViewAngle() {
    return ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, viewAngle)};
}

void Player::setViewAngle(Vec3 angle) {
    ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, viewAngle)} = angle;
}

std::string Player::getName() {
    Offset::Name name = ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, name)};
    *name.rbegin() = '\0';
    return name.data();
}

int Player::getTeamId() {
    return ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, team)};
}

float Player::getHealth() {
    return (float)ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, health)};
}

void Player::setHealth(float health) {
    ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, health)} = (int)health;
}

float Player::getArmor() {
    return (float)ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, armor)};
}

void Player::setArmor(float armor) {
    ProcessMemoryAccessor{ADDRESS(_this, Offset::Player, armor)} = (int)armor;
}

bool Player::operator==(const PlayerBasicInterface &other) const {
    return _this == dynamic_cast<const Player&>(other)._this;
}
