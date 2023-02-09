#include <sstream>
#include <iomanip>
#include "mem/memory_accessor.h"
#include "offset.h"
#include "item.h"

Item::Item(uint32_t ptr, std::string name)
        : _this(ptr), name(std::move(name)) {}

glm::vec3 Item::getPosition() {
    return MemoryAccessor<glm::vec3>{_this + hazedumper::netvars::m_vecOrigin};
}

std::string Item::getName() {
    return name;
}

std::string Item::getExtra() {
    return {};
}

int Item::getLevel() {
    return 0;
}

std::string PlantedC4::getExtra() {
    // don't show extra info if bomb is not ticking
    if (MemoryAccessor<uint8_t>{_this + hazedumper::netvars::m_bBombTicking} == 0) {
        return {};
    }

    float currentTime = MemoryAccessor<float>{
        _this + hazedumper::netvars::m_flLastBoneSetupTime
    };
    float explodeTime = MemoryAccessor<float>{
        _this + hazedumper::netvars::m_flC4Blow
    };
    float deltaTime = explodeTime - currentTime;
    if (deltaTime < 0) {
        return {};
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << deltaTime << "s";
    return ss.str();
}
