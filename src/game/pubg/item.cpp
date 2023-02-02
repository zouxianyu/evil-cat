#include "mem/memory_accessor.h"
#include "decrypt.h"
#include "structure.h"
#include "offset.h"
#include "item.h"

Item::Item(uint64_t address, ItemInfo info)
        : _this(address), info(std::move(info)) {}

glm::vec3 Item::getPosition() {
    uint64_t rootComponent = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            _this + Offset_RootComponent
    ));

    glm::vec3 position = MemoryAccessor<glm::vec3>(
            rootComponent + Offset_ComponentLocation
    );
    return position;
}

std::string Item::getName() {
    return info.name;
}

std::string Item::getExtra() {
    return {};
}

int Item::getLevel() {
    return info.level;
}
