#ifndef EVIL_CAT_MEM_BUFFER_POOL_H
#define EVIL_CAT_MEM_BUFFER_POOL_H

#include <unordered_map>
#include <optional>
#include <mutex>
#include <array>
#include "process_memory_interface.h"
#include "singleton.h"

#define ROUND_DOWN(a, b) ((uintptr_t)(a) & ~((uintptr_t)(b) - 1))
#define ROUND_UP(a, b) (((uintptr_t)(a) + ((uintptr_t)(b) - 1)) & ~((uintptr_t)(b) - 1))

using PageCache = std::array<uint8_t, CONF_BUFFER_POOL_CACHE_LINE_SIZE>;

class BufferPool : public Singleton<BufferPool> {

    std::unordered_map<gameptr_t, PageCache> cacheMap;

    std::mutex cacheMapMutex;

    std::optional<PageCache> getPageCache(gameptr_t alignedAddress, bool allocate);
public:

    bool read(gameptr_t address, void *buffer, gamesize_t size, bool cache);

    bool write(gameptr_t address, const void *buffer, gamesize_t size);

    bool refresh();
};

#endif //EVIL_CAT_MEM_BUFFER_POOL_H
