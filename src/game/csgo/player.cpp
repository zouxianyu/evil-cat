#include <map>
#include "magic_enum.h"
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
    const uint32_t viewAngleOffset = hazedumper::netvars::m_vecOrigin - sizeof(glm::vec3);
    return MemoryAccessor<glm::vec3>{_this + viewAngleOffset};
}

void Player::setViewAngle(glm::vec3 angle) {
    // not support
}

std::string Player::getName() {
    uint32_t id = MemoryAccessor<uint32_t>{_this + 0x64};

    using Name = std::array<char, 128>;
    Name name = MemoryAccessor<Name>{
            "engine.dll", hazedumper::signatures::dwClientState,
            {
                    hazedumper::signatures::dwClientState_PlayerInfo,
                    0x40,
                    0xC,
                    0x28 + (id - 1) * 0x34,
                    0x10
            }
    };
    *name.rbegin() = '\0';
    return name.data();
}

int Player::getTeamId() {
    return MemoryAccessor<int>{_this + hazedumper::netvars::m_iTeamNum};
}

float Player::getHealth() {
    return (float)MemoryAccessor<int>{_this + hazedumper::netvars::m_iHealth};
}

void Player::setHealth(float health) {
    // not support
}

float Player::getArmor() {
    // not support
    return 0.f;
}

void Player::setArmor(float armor) {
    // not support
}

glm::vec3 Player::getBoneById(int id) {
    glm::mat3x4 mem = MemoryAccessor<glm::mat3x4>{
            _this + hazedumper::netvars::m_dwBoneMatrix,
            {static_cast<gameptr_t>(id * 0x30)}
    };
    return {mem[0][3], mem[1][3], mem[2][3]};
}

using BoneMappingArray = std::array<int, magic_enum::enum_count<Bone>()>;

constexpr BoneMappingArray getBoneMapping_unknown() {
    return {};
}

constexpr BoneMappingArray getBoneMapping_ctm_idf() {
    BoneMappingArray map{};
    map[static_cast<int>(Bone::head)] = 8;
    map[static_cast<int>(Bone::neck)] = 7;
    map[static_cast<int>(Bone::leftShoulder)] = 11;
    map[static_cast<int>(Bone::rightShoulder)] = 41;
    map[static_cast<int>(Bone::leftElbow)] = 12;
    map[static_cast<int>(Bone::rightElbow)] = 42;
    map[static_cast<int>(Bone::leftHand)] = 13;
    map[static_cast<int>(Bone::rightHand)] = 43;
    map[static_cast<int>(Bone::spine)] = 5;
    map[static_cast<int>(Bone::hip)] = 0;
    map[static_cast<int>(Bone::leftHip)] = 70;
    map[static_cast<int>(Bone::rightHip)] = 77;
    map[static_cast<int>(Bone::leftKnee)] = 71;
    map[static_cast<int>(Bone::rightKnee)] = 78;
    map[static_cast<int>(Bone::leftFoot)] = 72;
    map[static_cast<int>(Bone::rightFoot)] = 79;
    return map;
}

constexpr BoneMappingArray getBoneMapping_tm_leet() {
    BoneMappingArray map{};
    map[static_cast<int>(Bone::head)] = 8;
    map[static_cast<int>(Bone::neck)] = 7;
    map[static_cast<int>(Bone::leftShoulder)] = 11;
    map[static_cast<int>(Bone::rightShoulder)] = 39;
    map[static_cast<int>(Bone::leftElbow)] = 12;
    map[static_cast<int>(Bone::rightElbow)] = 40;
    map[static_cast<int>(Bone::leftHand)] = 13;
    map[static_cast<int>(Bone::rightHand)] = 41;
    map[static_cast<int>(Bone::spine)] = 5;
    map[static_cast<int>(Bone::hip)] = 0;
    map[static_cast<int>(Bone::leftHip)] = 66;
    map[static_cast<int>(Bone::rightHip)] = 73;
    map[static_cast<int>(Bone::leftKnee)] = 67;
    map[static_cast<int>(Bone::rightKnee)] = 74;
    map[static_cast<int>(Bone::leftFoot)] = 68;
    map[static_cast<int>(Bone::rightFoot)] = 75;
    return map;
}

PlayerType Player::getPlayerType() {
    // get model name
    using ModelPath = std::array<char, 64>;
    ModelPath name = MemoryAccessor<ModelPath>{
            _this + 0x6C,
            {0x4}
    };
    *name.rbegin() = '\0';
    std::string modelName = name.data();

    // compare model name
    if (modelName.find("ctm_idf") != std::string::npos) {
        return PlayerType::ctm_idf;
    } else if (modelName.find("tm_leet") != std::string::npos) {
        return PlayerType::tm_leet;
    } else {
        return PlayerType::unknown;
    }
}

BoneArray Player::getBonePositions() {
    const static std::map<PlayerType, BoneMappingArray> boneMappings{
            {PlayerType::ctm_idf, getBoneMapping_ctm_idf()},
            {PlayerType::tm_leet, getBoneMapping_tm_leet()},
            {PlayerType::unknown, getBoneMapping_unknown()},
    };

    BoneArray bonePositions{};
    PlayerType type = getPlayerType();
    for (int i = 0; i < bonePositions.size(); i++) {
        bonePositions[i] = getBoneById(boneMappings.at(type)[i]);
    }
    return bonePositions;
}

Weapon Player::getWeapon() {
    return {};
}

bool Player::isVisible() {
    return true;
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
