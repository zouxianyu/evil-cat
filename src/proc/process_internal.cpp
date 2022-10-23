#include <windows.h>
#include "process_internal.h"

bool ProcessInternal::attach(const std::string &processName) {
    return true;
}

bool ProcessInternal::detach() {
    return true;
}

gameptr_t ProcessInternal::getModuleAddress(const std::string &moduleName) {
    HANDLE handle = GetModuleHandle(moduleName.c_str());
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return 0;
    }
    CloseHandle(handle);
    return reinterpret_cast<gameptr_t>(handle);
}

bool ProcessInternal::read(gameptr_t address, void *buffer, gamesize_t size) {
    try {
        // we cast the pointer to void * because the DLL to be injected must
        // have the same bitness as the game, so the cast make sense
        memcpy(buffer, reinterpret_cast<void *>(address), size);
        return true;
    } catch (...) {
        return false;
    }
}

bool ProcessInternal::write(gameptr_t address, const void *buffer, gamesize_t size) {
    try {
        // the reason why we cast the pointer to void * is the same as above
        memcpy(reinterpret_cast<void *>(address), buffer, size);
        return true;
    } catch (...) {
        return false;
    }
}
