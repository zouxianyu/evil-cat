#ifndef EVIL_CAT_MEM_BUFFER_POOL_H
#define EVIL_CAT_MEM_BUFFER_POOL_H

#include <map>
#include <unordered_map>
#include <optional>
#include <mutex>
#include <array>
#include "cache/registry.h"
#include "proc/process_interface.h"
#include "singleton.h"

#define ROUND_DOWN(a, b) ((gameptr_t)(a) & ~((gameptr_t)(b) - 1))
#define ROUND_UP(a, b) (((gameptr_t)(a) + ((gameptr_t)(b) - 1)) & ~((gameptr_t)(b) - 1))

using PageCache = std::array<uint8_t, CONF_BUFFER_POOL_CACHE_LINE_SIZE>;

class BufferPool : public Singleton<BufferPool>, public CacheRegistry {

    std::map<gameptr_t, PageCache> cacheMap;

    std::mutex cacheMapMutex;

    std::unordered_map<std::string, gameptr_t> moduleAddressMap;

    std::mutex moduleAddressMutex;

    std::optional<PageCache> getPageCache(gameptr_t alignedAddress, bool allocate);
public:

    bool read(gameptr_t address, void *buffer, gamesize_t size, bool cache);

    bool write(gameptr_t address, const void *buffer, gamesize_t size);

    gameptr_t getModuleAddress(const std::string &moduleName);

    void refresh() override;
};

#endif //EVIL_CAT_MEM_BUFFER_POOL_H
