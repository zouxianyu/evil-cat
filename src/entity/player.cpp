#include "mem/process_memory_accessor.h"
#include "player.h"

Player::Player(void *address)
        : _this(address) {
}

Vec3 Player::getHead() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this)+ offsetof(Player, head));
    ProcessMemoryAccessor<decltype(head)> accessor{address};
    return accessor.get();
}

Vec3 Player::getFeet() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this)+ offsetof(Player, feet));
    ProcessMemoryAccessor<decltype(feet)> accessor{address};
    return accessor.get();
}

Vec3 Player::getViewAngle() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this)+ offsetof(Player, viewAngle));
    ProcessMemoryAccessor<decltype(viewAngle)> accessor{address};
    return accessor.get();
}

void Player::setViewAngle(Vec3 angle) {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this)+ offsetof(Player, viewAngle));
    ProcessMemoryAccessor<decltype(viewAngle)> accessor{address};
    accessor.set(angle);
}

int Player::getHealth() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, health));
    ProcessMemoryAccessor<decltype(health)> accessor{address};
    return accessor.get();
}

void Player::setHealth(int health) {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, health));
    ProcessMemoryAccessor<decltype(health)> accessor{address};
    accessor.set(health);
}

int Player::getArmor() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, armor));
    ProcessMemoryAccessor<decltype(armor)> accessor{address};
    return accessor.get();
}

void Player::setArmor(int armor) {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, armor));
    ProcessMemoryAccessor<decltype(armor)> accessor{address};
    accessor.set(armor);
}

std::string Player::getName() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, name));
    ProcessMemoryAccessor<decltype(name)> accessor{address};
    return accessor.get().name;
}

uint8_t Player::getTeam() {
    void *address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_this) + offsetof(Player, team));
    ProcessMemoryAccessor<decltype(team)> accessor{address};
    return accessor.get();
}



