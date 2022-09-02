#ifndef ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H
#define ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H

#include <string>
#include <vector>
#include <stdexcept>
#include "proc/process_info.h"
#include "buffer_pool.h"


template<typename T>
class ProcessMemoryAccessor {
    T *address;
    bool cache;
public:
    ProcessMemoryAccessor(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor &operator=(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor(ProcessMemoryAccessor &&) = delete;

    ProcessMemoryAccessor &operator=(ProcessMemoryAccessor &&) = delete;

    explicit ProcessMemoryAccessor(
            std::string moduleName,
            uintptr_t moduleOffset,
            const std::vector<uintptr_t> &offsets = {},
            bool cache = true
    ) {
        void *moduleBase = nullptr;
        if (!ProcessInfo::getInstance().getModuleAddress(moduleName, moduleBase)) {
            throw std::runtime_error("Could not find module " + moduleName);
        }
        address = reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(moduleBase) + moduleOffset);
        for (uintptr_t offset: offsets) {
            BufferPool::getInstance().read(address, &address, sizeof(uintptr_t), cache);
            address = reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(address) + offset);
        }
        this->address = address;
        this->cache = cache;
    }

    explicit ProcessMemoryAccessor(
            T *address,
            const std::vector<uintptr_t> &offsets = {},
            bool cache = true
    ) {
        for (uintptr_t offset: offsets) {
            BufferPool::getInstance().read(address, &address, sizeof(uintptr_t), cache);
            address = reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(address) + offset);
        }
        this->address = address;
        this->cache = cache;
    }

    T get() {
        T value;
        if (!BufferPool::getInstance().read(address, &value, sizeof(T), cache)) {
            throw std::runtime_error("failed to read memory");
        }
        return value;
    }

    void set(const T &value) {
        if (!BufferPool::getInstance().write(address, &value, sizeof(T))) {
            throw std::runtime_error("failed to write memory");
        }
    }

    operator T() {
        return get();
    }

    ProcessMemoryAccessor<T> &operator=(const T &value) {
        set(value);
        return *this;
    }
};

#endif //ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H
