#ifndef EVIL_CAT_MEM_BUFFER_POOL_H
#define EVIL_CAT_MEM_BUFFER_POOL_H

#include <unordered_map>
#include <optional>
#include <mutex>
#include "type.h"

#define ROUND_DOWN(a, b) ((uintptr_t)(a) & ~((uintptr_t)(b) - 1))
#define ROUND_UP(a, b) (((uintptr_t)(a) + ((uintptr_t)(b) - 1)) & ~((uintptr_t)(b) - 1))

struct PageInfo {
    uint8_t buffer[BUFFER_POOL_CACHE_LINE_SIZE];
};

class BufferPool {

    std::unordered_map<void *, PageInfo> cacheMap;

    std::mutex cacheMapMutex;

    std::optional<PageInfo> getPageInfo(void *alignedAddress, bool allocate);

public:

    static BufferPool &getInstance();

    bool read(void *address, void *buffer, size_t size, bool cache);

    bool write(void *address, const void *buffer, size_t size);

    bool refresh();
};

#endif //EVIL_CAT_MEM_BUFFER_POOL_H
