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
    // if we don't want to use the cache, just read from the memory directly
    if (!cache) {
        return ProcessMemory::getInstance().read(address, buffer, size);
    }

    // use loop to read cache line by line
    while (size > 0) {
        uintptr_t addrAlign = ROUND_DOWN(address, BUFFER_POOL_CACHE_LINE_SIZE);

        // get cache info by the aligned read address
        std::optional<PageInfo> cachedAddress = getPageInfo(
                (void *) addrAlign,
                true
        );
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
#ifdef USE_BUFFER_POOL
    // copy this variable to avoid modifying the original one
    // because at last we will use the original one
    void *_address = address;
    const void *_buffer = buffer;
    size_t _size = size;

    // use loop to write cache line by line
    // we use write-through policy here,
    // because we want to matain the cache consistency
    while (_size > 0) {
        uintptr_t addrAlign = ROUND_DOWN(_address, BUFFER_POOL_CACHE_LINE_SIZE);

        // get cache info by the aligned read address
        std::optional<PageInfo> cachedAddress = getPageInfo((void *) addrAlign,
                                                            false);
        // get write size of the current cacheline
        size_t n = std::min(
                BUFFER_POOL_CACHE_LINE_SIZE - (addrAlign - (uintptr_t) _address),
                _size
        );

        if (cachedAddress.has_value()) {
            // copy the data from the buffer,
            // write it to the cache line
            memmove( reinterpret_cast<void *>(
                             reinterpret_cast<uintptr_t>(cachedAddress.value().buffer) +
                             (reinterpret_cast<uintptr_t>(_address) - addrAlign)
                     ),
                     _buffer,
                     n
            );
        }

        // adjust pointers
        _size -= n;
        _buffer = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_buffer) + n);
        _address = reinterpret_cast<void *>(addrAlign + BUFFER_POOL_CACHE_LINE_SIZE);
    }
#endif
    // we should write it to the memory anyway
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

std::optional<PageInfo>
BufferPool::getPageInfo(void *alignedAddress, bool allocate) {
    std::lock_guard lock(cacheMapMutex);

    // try to find a cache line which is in the cache map
    auto it = cacheMap.find(alignedAddress);
    if (it != cacheMap.end()) {
        return it->second;
    }

    // there's no cache line in the cache map
    // so if we don't want to allocate a new one,
    // we just return an empty optional
    if (!allocate) {
        return std::nullopt;
    }

    // allocate a new cache line and read it from the target address
    // from the next level interface
    PageInfo pageInfo{};
    if (!ProcessMemory::getInstance().read(alignedAddress, pageInfo.buffer, BUFFER_POOL_CACHE_LINE_SIZE)) {
        return std::nullopt;
    }

    // insert it in to the cache map
    // so that we can use it next time
    cacheMap.insert(std::make_pair(alignedAddress, pageInfo));

    return pageInfo;
}


