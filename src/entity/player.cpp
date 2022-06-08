#include "mem/process_memory_accessor.h"
#include "entity/offset/player.h"
#include "player.h"

Player::Player(void *address)
        : _this(address) {
}

Vec3 Player::getHead() {
    ProcessMemoryAccessor accessor{ADDRESS(_this, offset::Player, head)};
    return accessor;
}

Vec3 Player::getFeet() {
    ProcessMemoryAccessor accessor{ADDRESS(_this, offset::Player, feet)};
    return accessor;
}

Vec3 Player::getViewAngle() {
    return ProcessMemoryAccessor{ADDRESS(_this, offset::Player, viewAngle)};
}

void Player::setViewAngle(Vec3 angle) {
    ProcessMemoryAccessor{ADDRESS(_this, offset::Player, viewAngle)} = angle;
}

int Player::getHealth() {
    return ProcessMemoryAccessor{ADDRESS(_this, offset::Player, health)};
}

void Player::setHealth(int health) {
    ProcessMemoryAccessor{ADDRESS(_this, offset::Player, health)} = health;
}

int Player::getArmor() {
    return ProcessMemoryAccessor{ADDRESS(_this, offset::Player, armor)};
}

void Player::setArmor(int armor) {
    ProcessMemoryAccessor{ADDRESS(_this, offset::Player, armor)} = armor;
}

std::string Player::getName() {
    Name name = ProcessMemoryAccessor{ADDRESS(_this, offset::Player, name)};
    return name.name;
}

uint8_t Player::getTeam() {
    return ProcessMemoryAccessor{ADDRESS(_this, offset::Player, team)};
}
