#include <windows.h>
#include "process_memory_internal.h"

bool ProcessMemoryInternal::attach(const std::string &processName) {
    // no need to attach
    return true;
}

bool ProcessMemoryInternal::detach() {
    // no need to detach
    return true;
}

bool ProcessMemoryInternal::read(gameptr_t address, void *buffer, size_t size) {
    try {
        // we cast the pointer to void * because the DLL to be injected must
        // have the same bitness as the game, so the cast make sense
        memcpy(buffer, reinterpret_cast<void *>(address), size);
        return true;
    } catch (...) {
        return false;
    }
}

bool ProcessMemoryInternal::write(gameptr_t address, const void *buffer, size_t size) {
    try {
        // the reason why we cast the pointer to void * is the same as above
        memcpy(reinterpret_cast<void *>(address), buffer, size);
        return true;
    } catch (...) {
        return false;
    }
}
