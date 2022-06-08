#include <windows.h>
#include "process_memory_internal.h"

ProcessMemory &ProcessMemoryInternal::getInstance() {
    static ProcessMemoryInternal instance;
    return instance;
}

bool ProcessMemoryInternal::attach(const std::string &processName) {
    // no need to attach
    return true;
}

bool ProcessMemoryInternal::detach() {
    // no need to detach
    return true;
}

bool ProcessMemoryInternal::read(void *address, void *buffer, size_t size) {
    try {
        memcpy(buffer, address, size);
        return true;
    } catch (...) {
        return false;
    }
}

bool ProcessMemoryInternal::write(void *address, const void *buffer, size_t size) {
    try {
        memcpy(address, buffer, size);
        return true;
    } catch (...) {
        return false;
    }
}
