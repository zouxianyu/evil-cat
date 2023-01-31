#ifndef EVIL_CAT_GAME_PUBG_NAME_POOL_H
#define EVIL_CAT_GAME_PUBG_NAME_POOL_H

#include <string>
#include <cstdint>

namespace PUBG::NamePool {
    std::string getName(uint32_t id);
}

namespace PUBG {
    using namespace PUBG::NamePool;
}

#endif //EVIL_CAT_GAME_PUBG_NAME_POOL_H
