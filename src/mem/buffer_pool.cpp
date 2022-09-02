#include "process_memory.h"
#include "buffer_pool.h"

BufferPool &BufferPool::getInstance() {
    static BufferPool instance;
    return instance;
}

bool BufferPool::read(void *address, void *buffer, size_t size, bool cache) {
#ifndef USE_BUFFER_POOL
    return ProcessMemory::getInstance().read(address, buffer, size);
#else
    // use loop to read cache line by line
    while (size > 0) {
        uintptr_t addrAlign = ROUND_DOWN(address, BUFFER_POOL_CACHE_LINE_SIZE);

        // get cache info by the aligned read address
        std::optional<PageInfo> cachedAddress = getPageInfo((void *) addrAlign);
        if (!cachedAddress.has_value()) {
            return false;
        }

        // get read size of the current cacheline
        size_t n = std::min(
                BUFFER_POOL_CACHE_LINE_SIZE - (addrAlign - (uintptr_t) address),
                size
        );

        // copy data to the buffer
        memmove(buffer,
                reinterpret_cast<void *>(
                        reinterpret_cast<uintptr_t>(cachedAddress.value().buffer) +
                        (reinterpret_cast<uintptr_t>(address) - addrAlign)
                    ),
                n);

        // adjust pointers
        size -= n;
        buffer = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(buffer) + n);
        address = reinterpret_cast<void *>(addrAlign + BUFFER_POOL_CACHE_LINE_SIZE);
    }

    return true;

#endif
}

bool BufferPool::write(void *address, const void *buffer, size_t size) {
    return ProcessMemory::getInstance().write(address, buffer, size);
}

bool BufferPool::refresh() {
#ifndef USE_BUFFER_POOL
    return true;
#else
    std::lock_guard lock(cacheMapMutex);
    cacheMap.clear();
    return true;
#endif
}

std::optional<PageInfo> BufferPool::getPageInfo(void *alignedAddress) {
    std::lock_guard lock(cacheMapMutex);

    auto it = cacheMap.find(alignedAddress);
    if (it != cacheMap.end()) {
        return it->second;
    }

    PageInfo pageInfo{};
    if (!ProcessMemory::getInstance().read(alignedAddress, pageInfo.buffer, BUFFER_POOL_CACHE_LINE_SIZE)) {
        return std::nullopt;
    }

    cacheMap.insert(std::make_pair(alignedAddress, pageInfo));
    return pageInfo;
}


