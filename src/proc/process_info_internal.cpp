#include <windows.h>
#include "process_info_internal.h"

ProcessInfo &ProcessInfoInternal::getInstance() {
    static ProcessInfoInternal instance;
    return instance;
}

bool ProcessInfoInternal::attach(const std::string &processName) {
    return true;
}

bool ProcessInfoInternal::detach() {
    return true;
}

bool ProcessInfoInternal::getModuleAddress(const std::string &moduleName, void *&address) {
    if (moduleName.empty()) {
        return false;
    }

    // if the module is already recorded, return its base _this
    auto it = modulesAddress.find(moduleName);
    if (it != modulesAddress.end()) {
        address = it->second;
        return true;
    }

    // otherwise, find the module's base _this via GetModuleHandle
    HANDLE handle = GetModuleHandle(moduleName.c_str());
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    address = reinterpret_cast<void *>(handle);
    CloseHandle(handle);

    // add the _this to our map
    modulesAddress[moduleName] = address;
    return true;
}

bool ProcessInfoInternal::refresh() {
    modulesAddress.clear();
    return true;
}
