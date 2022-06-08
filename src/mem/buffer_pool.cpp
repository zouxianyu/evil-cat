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
    if (!cache) {
        return ProcessMemory::getInstance().read(address, buffer, size);
    }

    uintptr_t baseAddress, startAddress, endAddress;
    size_t tempSize = 0;

    // prefix part
    baseAddress = ROUND_DOWN(address, BUFFER_POOL_CACHE_LINE_SIZE);
    startAddress = (uintptr_t) address;
    endAddress = std::min(ROUND_UP(address, BUFFER_POOL_CACHE_LINE_SIZE), startAddress + size);
    if (startAddress != endAddress) {
        std::optional<PageInfo> cachedAddress = getPageInfo((void *) baseAddress);
        if (!cachedAddress.has_value()) {
            return false;
        }
        size_t copySize = endAddress - startAddress;
        size_t copyOffset = startAddress - baseAddress;
        uintptr_t copyAddress = (uintptr_t) cachedAddress.value().buffer + copyOffset;
        memcpy((void *) ((uintptr_t) buffer + tempSize), (void *) copyAddress, copySize);
        tempSize += copySize;
    }

    if (tempSize == size) {
        return true;
    }

    // middle part
    startAddress = ROUND_UP(address, BUFFER_POOL_CACHE_LINE_SIZE);
    endAddress = ROUND_DOWN((uintptr_t) address + size, BUFFER_POOL_CACHE_LINE_SIZE);
    for (uintptr_t i = startAddress; i < endAddress; i += BUFFER_POOL_CACHE_LINE_SIZE) {
        std::optional<PageInfo> cachedAddress = getPageInfo((void *) i);
        if (!cachedAddress.has_value()) {
            return false;
        }
        memcpy((void *) ((uintptr_t) buffer + tempSize), cachedAddress.value().buffer, BUFFER_POOL_CACHE_LINE_SIZE);
        tempSize += BUFFER_POOL_CACHE_LINE_SIZE;
    }

    if (tempSize == size) {
        return true;
    }

    // suffix part
    startAddress = ROUND_DOWN((uintptr_t) address + size, BUFFER_POOL_CACHE_LINE_SIZE);
    endAddress = (uintptr_t) address + size;
    if (startAddress != endAddress) {
        std::optional<PageInfo> cachedAddress = getPageInfo((void *) startAddress);
        if (!cachedAddress.has_value()) {
            return false;
        }
        size_t copySize = endAddress - startAddress;
        memcpy((void *) ((uintptr_t) buffer + tempSize), cachedAddress.value().buffer, copySize);
        tempSize += copySize;
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
    cacheMap.clear();
    return true;
#endif
}

std::optional<PageInfo> BufferPool::getPageInfo(void *alignedAddress) {

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


