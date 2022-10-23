#include "mem/memory_accessor.h"
#include "offset/player.h"
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

glm::vec3 Player::getBonePosition(Bone boneType) {
    switch (boneType) {
        case Bone::head: {
            glm::vec3 head = MemoryAccessor<glm::vec3>{_this + Offset::Player::head};
            head.z += 0.25f;
            return head;
        }
        default: {
            return {0, 0, 0};
        }
    }
}

bool Player::operator==(const PlayerInterface &other) const {
    return _this == dynamic_cast<const Player&>(other)._this;
}
