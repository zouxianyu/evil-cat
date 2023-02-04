#include "mem/memory_accessor.h"
#include "offset.h"
#include "player.h"

Player::Player(gameptr_t address)
        : _this(address) {
}

glm::vec3 Player::getPosition() {
    return MemoryAccessor<glm::vec3>{_this + Offset::Player::feet};
}

float Player::getHeight() {
    glm::vec3 feet = getPosition();
    glm::vec3 head = getCameraPosition();
    return head.z - feet.z + 1.0f;
}

glm::vec3 Player::getCameraPosition() {
    return MemoryAccessor<glm::vec3>{_this + Offset::Player::head};
}

glm::vec3 Player::getViewAngle() {
    return MemoryAccessor<glm::vec3>{_this + Offset::Player::viewAngle};
}

void Player::setViewAngle(glm::vec3 angle) {
    MemoryAccessor<glm::vec3>{_this + Offset::Player::viewAngle} = angle;
}

std::string Player::getName() {
    Offset::Name name = MemoryAccessor<Offset::Name>{_this + Offset::Player::name};
    *name.rbegin() = '\0';
    return name.data();
}

int Player::getTeamId() {
    return (int)MemoryAccessor<uint8_t>{_this + Offset::Player::team};
}

float Player::getHealth() {
    return (float)MemoryAccessor<int>{_this + Offset::Player::health};
}

void Player::setHealth(float health) {
    MemoryAccessor<int>{_this + Offset::Player::health} = (int)health;
}

float Player::getArmor() {
    return (float)MemoryAccessor<int>{_this + Offset::Player::armor};
}

void Player::setArmor(float armor) {
    MemoryAccessor<int>{_this + Offset::Player::armor} = (int)armor;
}

BoneArray Player::getBonePositions() {
    BoneArray bonePositions{};
    bonePositions[static_cast<int>(Bone::head)] =
            MemoryAccessor<glm::vec3>{_this + Offset::Player::head};

    return bonePositions;
}

Weapon Player::getWeapon() {
    return {};
}

bool Player::isVisible() {
    return true;
}

bool Player::operator==(const PlayerInterface &other) const {
    return _this == dynamic_cast<const Player&>(other)._this;
}
