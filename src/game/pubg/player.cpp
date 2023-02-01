#include "mem/memory_accessor.h"
#include "decrypt.h"
#include "helper.h"
#include "structure.h"
#include "offset.h"
#include "player.h"

Player::Player(gameptr_t address)
        : _this(address) {
}

glm::vec3 Player::getPosition() {
    uint64_t rootComponent = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            _this + Offset_RootComponent
    ));

    // ComponentLocation = ComponentToWorld.Translation
    glm::vec3 position = MemoryAccessor<glm::vec3>(
            rootComponent + Offset_ComponentLocation
    );
    position.z -= 90.0f;
    return position;
}

float Player::getHeight() {
    return 180.0f;
}

glm::vec3 Player::getCameraPosition() {
    // unused
    return {};
}

glm::vec3 Player::getViewAngle() {
    uint64_t mesh = MemoryAccessor<uint64_t>(_this + Offset_Mesh);
    uint64_t animScriptInstance = MemoryAccessor<uint64_t>(mesh + Offset_AnimScriptInstance);
    return MemoryAccessor<glm::vec3>(animScriptInstance + Offset_ControlRotation_CP);
}

void Player::setViewAngle(glm::vec3 angle) {
    // unused
}

std::string Player::getName() {
    FString str = MemoryAccessor<FString>(_this + Offset_CharacterName);
    return PUBG::toString(str);
}

int Player::getTeamId() {
    return MemoryAccessor<int>(_this + Offset_LastTeamNum);
}

float Player::getHealth() {
    float health = MemoryAccessor<float>(_this + Offset_Health);
    float groggyHealth = MemoryAccessor<float>(_this + Offset_GroggyHealth);
    return health > 0.0f ? health : groggyHealth;
}

void Player::setHealth(float health) {
    // unused
}

float Player::getArmor() {
    // unused
    return {};
}

void Player::setArmor(float armor) {
    // unused
}

// 1. https://www.unknowncheats.me/forum/playerunknown-s-battlegrounds/229574-pubg-skeleton-bone-esp.html
// 2. lily
BoneArray Player::getBonePositions() {
    BoneArray bonePositions{};

    uint64_t mesh = MemoryAccessor<uint64_t>(_this + Offset_Mesh);
    FTransform componentToWorld = MemoryAccessor<FTransform>(
            mesh + Offset_ComponentToWorld
    );
    TArray boneTransforms = MemoryAccessor<TArray>(mesh + Offset_BoneSpaceTransforms);

    for (int i = 0; i < bonePositions.size(); i++) {
        FTransform boneTransform = MemoryAccessor<FTransform>(
                boneTransforms.ptr +
                PUBG::getBoneIndex(static_cast<Bone>(i)) * sizeof(FTransform)
        );

        bonePositions[i] = (boneTransform * componentToWorld).translation;
    }

    return bonePositions;
}

bool Player::operator==(const PlayerInterface &other) const {
    return _this == dynamic_cast<const Player&>(other)._this;
}

glm::vec3 LocalPlayer::getCameraPosition() {
    CameraInfo camera = PUBG::getCameraInfo();
    return camera.location;
}

glm::vec3 LocalPlayer::getViewAngle() {
    CameraInfo camera = PUBG::getCameraInfo();
    return camera.viewAngle;
}

void LocalPlayer::setViewAngle(glm::vec3 angle) {
    PUBG::setCameraRotation(angle);
}