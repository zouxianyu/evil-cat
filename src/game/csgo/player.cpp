#include "mem/memory_accessor.h"
#include "offset.h"
#include "player.h"

Player::Player(gameptr_t address)
        : _this(address) {
}

glm::vec3 Player::getPosition() {
    return MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_vecOrigin};
}

float Player::getHeight() {
    glm::vec3 vec = MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_vecViewOffset};
    return vec.z + 15.f;
}

glm::vec3 Player::getCameraPosition() {
    return MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_vecOrigin}.get() +
           MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_vecViewOffset}.get();
}

glm::vec3 Player::getViewAngle() {
    return MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_viewAngle};
}

void Player::setViewAngle(glm::vec3 angle) {
    // not support
    return;
}

std::string Player::getName() {
    return "player";
}

int Player::getTeamId() {
    return MemoryAccessor<int>{_this + hazedumper::netvars::m_iTeamNum};
}

float Player::getHealth() {
    return MemoryAccessor<int>{_this + hazedumper::netvars::m_iHealth};
}

void Player::setHealth(float health) {
    // not support
    return;
}

float Player::getArmor() {
    // not support
    return 0.f;
}

void Player::setArmor(float armor) {
    // not support
    return;
}

glm::vec3 Player::getBoneById(int id) {
    glm::mat3x4 mem = MemoryAccessor<glm::mat3x4>{
            _this + hazedumper::netvars::m_dwBoneMatrix,
            {static_cast<gameptr_t>(id * 0x30)}
    };
    return {mem[0][3], mem[1][3], mem[2][3]};
}

glm::vec3 Player::getBonePosition(Bone boneType) {
    switch (boneType) {
        case Bone::head:
            return getBoneById(8);
        case Bone::neck:
            return getBoneById(7);
        case Bone::leftShoulder:
            return getBoneById(11);
        case Bone::rightShoulder:
            return getBoneById(41);
        case Bone::leftElbow:
            return getBoneById(12);
        case Bone::rightElbow:
            return getBoneById(42);
        case Bone::leftHand:
            return getBoneById(13);
        case Bone::rightHand:
            return getBoneById(43);
        case Bone::spine:
            return getBoneById(5);
        case Bone::hip:
            return getBoneById(0);
        case Bone::leftHip:
            return getBoneById(70);
        case Bone::rightHip:
            return getBoneById(77);
        case Bone::leftKnee:
            return getBoneById(71);
        case Bone::rightKnee:
            return getBoneById(78);
        case Bone::leftFoot:
            return getBoneById(72);
        case Bone::rightFoot:
            return getBoneById(79);
        default:
            return getBoneById(8);
    }
}

bool Player::operator==(const PlayerInterface &other) const {
    return _this == dynamic_cast<const Player &>(other)._this;
}

void LocalPlayer::setViewAngle(glm::vec3 angle) {
    // because we cannot modify the view angle inside the player object
    // we can only modify it inside the engine.dll

    // recoil control
    glm::vec3 punchAngle = MemoryAccessor<glm::vec3>{
            _this + hazedumper::netvars::m_aimPunchAngle
    };
    angle -= punchAngle * 2.f;

    // clamp the angle
    angle.x = glm::clamp(angle.x, -89.f, 89.f);
    angle.y = glm::clamp(angle.y, -180.f, 180.f);
    angle.z = 0.f;

    // write it to the local player view angle
    MemoryAccessor<glm::vec3>{
            "engine.dll", hazedumper::signatures::dwClientState,
            {hazedumper::signatures::dwClientState_ViewAngles}
    } = angle;
}

glm::vec3 LocalPlayer::getViewAngle() {
    return MemoryAccessor<glm::vec3>{
            "engine.dll", hazedumper::signatures::dwClientState,
            {hazedumper::signatures::dwClientState_ViewAngles}
    };
}
