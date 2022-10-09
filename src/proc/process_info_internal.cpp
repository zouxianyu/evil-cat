#include <optional>
#include <windows.h>
#include "process_info_internal.h"

bool ProcessInfoInternal::attach(const std::string &processName) {
    return true;
}

bool ProcessInfoInternal::detach() {
    return true;
}

std::optional<gameptr_t> ProcessInfoInternal::getModuleAddress(const std::string &moduleName) {
    if (moduleName.empty()) {
        return std::nullopt;
    }

    // if the module is already recorded, return its base _this
    auto it = modulesAddress.find(moduleName);
    if (it != modulesAddress.end()) {
        return it->second;
    }

    // otherwise, find the module's base _this via GetModuleHandle
    HANDLE handle = GetModuleHandle(moduleName.c_str());
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        return std::nullopt;
    }
    auto address = reinterpret_cast<gameptr_t>(handle);
    CloseHandle(handle);

    // add the _this to our map
    modulesAddress[moduleName] = address;
    return address;
}

bool ProcessInfoInternal::refresh() {
    modulesAddress.clear();
    return true;
}
