#include <algorithm>
#include "module.h"
#include "buffer_pool.h"

bool BufferPool::read(gameptr_t address, void *buffer, gamesize_t size, bool cache) {
#ifndef CONF_USE_BUFFER_POOL
    return Module::processMemory->read(address, buffer, size);
#else
    // if we don't want to use the cache, just read from the memory directly
    if (!cache) {
        return Module::processMemory->read(address, buffer, size);
    }

    // use loop to read cache line by line
    while (size > 0) {
        gameptr_t addrAlign = ROUND_DOWN(address, CONF_BUFFER_POOL_CACHE_LINE_SIZE);

        // get cache info by the aligned read address
        std::optional<PageCache> cachedAddress = getPageCache(addrAlign, true);
        if (!cachedAddress) {
            return false;
        }

        // get read size of the current cacheline
        gamesize_t n = (std::min)(
                CONF_BUFFER_POOL_CACHE_LINE_SIZE - (address - addrAlign),
                size
        );

        // copy data to the buffer
        memmove(buffer,
                reinterpret_cast<void *>(
                        reinterpret_cast<uintptr_t>(&*cachedAddress) + (address - addrAlign)
                ),
                n
        );

        // adjust pointers
        size -= n;
        buffer = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(buffer) + n);
        address = addrAlign + CONF_BUFFER_POOL_CACHE_LINE_SIZE;
    }

    return true;
#endif
}

bool BufferPool::write(gameptr_t address, const void *buffer, gamesize_t size) {
#ifdef CONF_USE_BUFFER_POOL
    // copy this variable to avoid modifying the original one
    // because at last we will use the original one
    gameptr_t _address = address;
    const void *_buffer = buffer;
    gamesize_t _size = size;

    // use loop to write cache line by line
    // we use write-through policy here,
    // because we want to matain the cache consistency
    while (_size > 0) {
        gameptr_t addrAlign = ROUND_DOWN(_address, CONF_BUFFER_POOL_CACHE_LINE_SIZE);

        // get cache info by the aligned read address
        std::optional<PageCache> cachedAddress = getPageCache(addrAlign, false);

        // get write size of the current cacheline
        gamesize_t n = (std::min)(
                CONF_BUFFER_POOL_CACHE_LINE_SIZE - (_address - addrAlign),
                _size
        );

        if (cachedAddress) {
            // copy the data from the buffer,
            // write it to the cache line
            memmove(reinterpret_cast<void *>(
                            reinterpret_cast<uintptr_t>(&*cachedAddress) + (_address - addrAlign)
                    ),
                    _buffer,
                    n
            );
        }

        // adjust pointers
        _size -= n;
        _buffer = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(_buffer) + n);
        _address = addrAlign + CONF_BUFFER_POOL_CACHE_LINE_SIZE;
    }
#endif
    // we should write it to the memory anyway
    return Module::processMemory->write(address, buffer, size);
}

bool BufferPool::refresh() {
#ifndef CONF_USE_BUFFER_POOL
    return true;
#else
    std::lock_guard lock(cacheMapMutex);
    cacheMap.clear();
    return true;
#endif
}

std::optional<PageCache>
BufferPool::getPageCache(gameptr_t alignedAddress, bool allocate) {
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
    PageCache pageCache;
    if (!Module::processMemory->read(
            alignedAddress,
            &pageCache,
            CONF_BUFFER_POOL_CACHE_LINE_SIZE
    )) {
        return std::nullopt;
    }

    // insert it in to the cache map
    // so that we can use it next time
    cacheMap.insert(std::make_pair(alignedAddress, pageCache));

    return pageCache;
}


