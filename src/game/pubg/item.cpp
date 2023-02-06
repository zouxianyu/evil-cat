#include "mem/memory_accessor.h"
#include "decrypt.h"
#include "structure.h"
#include "offset.h"
#include "item.h"

ItemActor::ItemActor(uint64_t address, ItemInfo info)
        : _this(address), info(std::move(info)) {}

glm::vec3 ItemActor::getPosition() {
    uint64_t rootComponent = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            _this + Offset_RootComponent
    ));

    glm::vec3 position = MemoryAccessor<glm::vec3>(
            rootComponent + Offset_ComponentLocation
    );
    return position;
}

std::string ItemActor::getName() {
    return info.name;
}

std::string ItemActor::getExtra() {
    return {};
}

int ItemActor::getLevel() {
    return info.level;
}

glm::vec3 ItemComponent::getPosition() {
    glm::vec3 position = MemoryAccessor<glm::vec3>(
            _this + Offset_ComponentLocation
    );
    return position;
}
