#ifndef ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H
#define ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H

#include <string>
#include <vector>
#include <stdexcept>
#include "proc/process_info.h"
#include "buffer_pool.h"


template<typename T>
class ProcessMemoryAccessor {

    void *address;
    bool cache;
public:
    ProcessMemoryAccessor(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor &operator=(const ProcessMemoryAccessor &) = delete;

    ProcessMemoryAccessor(ProcessMemoryAccessor &&) = delete;

    ProcessMemoryAccessor &operator=(ProcessMemoryAccessor &&) = delete;

    ProcessMemoryAccessor() = delete;

    explicit ProcessMemoryAccessor(std::string moduleName, uintptr_t moduleOffset,
                                   const std::vector<uintptr_t> &offsets = {}, bool cache = true) {
        void *moduleBase = nullptr;
        if (!ProcessInfo::getInstance().getModuleAddress(moduleName, moduleBase)) {
            throw std::runtime_error("Could not find module " + moduleName);
        }
        address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(moduleBase) + moduleOffset);
        for (unsigned int offset: offsets) {
//            ProcessMemory::getInstance().read(address, &address, sizeof(uintptr_t));
            BufferPool::getInstance().read(address, &address, sizeof(uintptr_t), cache);
            address = reinterpret_cast<void *>( reinterpret_cast<uintptr_t>(address) + offset);
        }
        this->address = address;
        this->cache = cache;
    }

    explicit ProcessMemoryAccessor(void *address, const std::vector<uintptr_t> &offsets = {}, bool cache = true) {
        for (unsigned int offset: offsets) {
//            ProcessMemory::getInstance().read(address, &address, sizeof(uintptr_t));
            BufferPool::getInstance().read(address, &address, sizeof(uintptr_t), cache);
            address = reinterpret_cast<void *>( reinterpret_cast<uintptr_t>(address) + offset);
        }
        this->address = address;
        this->cache = cache;
    }

    T get() {
        T value;
//        if (!ProcessMemory::getInstance().read(address, &value, sizeof(T))) {
        if (!BufferPool::getInstance().read(address, &value, sizeof(T), cache)) {
            throw std::runtime_error("failed to read memory");
        }
        return value;
    }

    void set(const T &value) {
//        if (!ProcessMemory::getInstance().write(address, &value, sizeof(T))) {
        if (!BufferPool::getInstance().write(address, &value, sizeof(T))) {
            throw std::runtime_error("failed to write memory");
        }
    }
};

#endif //ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_ACCESSOR_H
