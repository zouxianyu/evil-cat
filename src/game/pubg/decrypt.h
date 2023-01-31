#ifndef EVIL_CAT_GAME_PUBG_DECRYPT_H
#define EVIL_CAT_GAME_PUBG_DECRYPT_H

#include <memory>
#include <windows.h>

namespace PUBG::Decrypt {
    uint64_t decryptPtr(uint64_t v);

    uint32_t decryptId(uint32_t v);
}

namespace PUBG {
    using namespace PUBG::Decrypt;
}

#endif //EVIL_CAT_GAME_PUBG_DECRYPT_H
