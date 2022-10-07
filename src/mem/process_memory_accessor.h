#ifndef EVIL_CAT_MEM_PROCESS_MEMORY_ACCESSOR_H
#define EVIL_CAT_MEM_PROCESS_MEMORY_ACCESSOR_H

#include <string>
#include <vector>
#include <type_traits>
#include "module_config.h"
#include "module.h"
#include "game_ptr.h"
#include "log.h"
#include "buffer_pool.h"


template<typename T>
class ProcessMemoryAccessor {
    // check T's type
    // if T is not a pod type, we cannot directly read/write it
    static_assert(std::is_pod_v<T>, "T must be a POD type");

    gameptr_t address;

    bool cache;

    bool bad;
public:

    ProcessMemoryAccessor(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor &operator=(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor(ProcessMemoryAccessor &&) = delete;

    ProcessMemoryAccessor &operator=(ProcessMemoryAccessor &&) = delete;

    explicit ProcessMemoryAccessor(
            std::string moduleName,
            gameptr_t moduleOffset,
            const std::vector<gameptr_t> &offsets = {},
            bool cache = true
    ) {
        gameptr_t moduleBase{};
        if (auto result = Module::processInfo->getModuleAddress(moduleName)) {
            moduleBase = *result;
            bad = false;
        } else {
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

    explicit ProcessMemoryAccessor(
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

    ProcessMemoryAccessor<T> &operator=(const T &value) {
        set(value);
        return *this;
    }

    ProcessMemoryAccessor<T> &operator=(T &&value) {
        set(value);
        return *this;
    }
};

#endif //EVIL_CAT_MEM_PROCESS_MEMORY_ACCESSOR_H
