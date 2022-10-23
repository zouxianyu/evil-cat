#ifndef EVIL_CAT_MEM_MEMORY_ACCESSOR_H
#define EVIL_CAT_MEM_MEMORY_ACCESSOR_H

#include <string>
#include <vector>
#include <type_traits>
#include "module_config.h"
#include "module.h"
#include "log.h"
#include "buffer_pool.h"


template<typename T>
class MemoryAccessor {
    // check T's type
    // if T is not a pod type, we cannot directly read/write it
    static_assert(std::is_trivial_v<T>, "T must be a POD type");

    gameptr_t address{};

    bool cache{};

    bool bad{};
public:

    MemoryAccessor(const MemoryAccessor &) = delete;

    MemoryAccessor &operator=(const MemoryAccessor &) = delete;

    MemoryAccessor(MemoryAccessor &&) = delete;

    MemoryAccessor &operator=(MemoryAccessor &&) = delete;

    explicit MemoryAccessor(
            std::string moduleName,
            gameptr_t moduleOffset,
            const std::vector<gameptr_t> &offsets = {},
            bool cache = true
    ) {
        gameptr_t moduleBase = BufferPool::getInstance().getModuleAddress(moduleName);
        if (!moduleBase) {
            LOG_DEBUG << "Failed to get module address for " << moduleName << std::endl;
            bad = true;
            return;
        }
        address = moduleBase + moduleOffset;
        for (gameptr_t offset: offsets) {
            if (!BufferPool::getInstance().read(address, &address, sizeof(gameptr_t), cache)) {
                LOG_DEBUG << "Failed to read address at " << std::hex << address << std::endl;
                bad = true;
                return;
            }
            address += offset;
        }
        this->address = address;
        this->cache = cache;
    }

    explicit MemoryAccessor(
            gameptr_t address,
            const std::vector<gameptr_t> &offsets = {},
            bool cache = true
    ) {
        for (gameptr_t offset: offsets) {
            if (!BufferPool::getInstance().read(address, &address, sizeof(gameptr_t), cache)) {
                LOG_DEBUG << "Failed to read address at " << std::hex << address << std::endl;
                bad = true;
                return;
            }
            address += offset;
        }
        this->address = address;
        this->cache = cache;
    }

    T get() const {
        T value{};
        if (bad || !BufferPool::getInstance().read(address, &value, sizeof(T), cache)) {
            return {};
        }
        return value;
    }

    void set(const T &value) {
        if (bad) {
            return;
        }
        BufferPool::getInstance().write(address, &value, sizeof(T));
    }

    operator T() const {
        return get();
    }

    MemoryAccessor<T> &operator=(const T &value) {
        set(value);
        return *this;
    }

    MemoryAccessor<T> &operator=(T &&value) {
        set(value);
        return *this;
    }
};

#endif //EVIL_CAT_MEM_MEMORY_ACCESSOR_H
