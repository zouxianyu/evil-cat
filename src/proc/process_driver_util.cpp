#include <vector>
#include <filesystem>
#include "process_helper.h"
#include "process_driver_util.h"

bool ProcessDriverUtil::openDriver() {
    driverHandle.reset(CreateFile(
            DEVICE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    ));
    return driverHandle.is_valid();
}

void ProcessDriverUtil::closeDriver() {
    driverHandle.reset();
}

bool ProcessDriverUtil::attach(const std::string &processName) {
    std::vector<uint32_t> pids = ProcessHelper::getProcessIdsByName(processName);
    if (pids.empty()) {
        return false;
    }

    pid = pids[0];

    // try to open driver
    if (openDriver()) {
        return true;
    }

    // try to load driver
    if (!driverLoader->load(std::filesystem::absolute(DRIVER_PATH).string(), DISPLAY_NAME)) {
        return false;
    }

    // try to open driver again
    return openDriver();
}

bool ProcessDriverUtil::detach() {
    pid = 0;
    closeDriver();
    // unload driver
    return driverLoader->unload(DISPLAY_NAME);
}

gameptr_t ProcessDriverUtil::getModuleAddress(const std::string &moduleName) {
    if (!driverHandle.is_valid()) {
        return 0;
    }

    // alloc request
    auto mem = std::make_unique<uint8_t[]>(
            sizeof(GetModuleAddressRequest) + moduleName.length()
    );
    auto request = reinterpret_cast<GetModuleAddressRequest *>(mem.get());

    // fill request
    request->size = sizeof(GetModuleAddressRequest) + moduleName.length();
    request->pid = pid;
    memcpy(request->moduleName, moduleName.c_str(), moduleName.length());

    // send request
    DWORD bytesReturned = 0;
    uint64_t moduleAddress = 0;
    if (!DeviceIoControl(
            driverHandle.get(),
            IOCTL_PROC_MODULE,
            request,
            request->size,
            &moduleAddress,
            sizeof(moduleAddress),
            &bytesReturned,
            nullptr
    )) {
        return 0;
    }

    return moduleAddress;
}

bool ProcessDriverUtil::read(gameptr_t address, void *buffer, gamesize_t size) {
    if (!driverHandle.is_valid()) {
        return false;
    }

    // alloc request
    auto mem = std::make_unique<uint8_t[]>(sizeof(ReadMemoryRequest));
    auto request = reinterpret_cast<ReadMemoryRequest *>(mem.get());

    // fill request
    request->size = sizeof(ReadMemoryRequest);
    request->pid = pid;
    request->address = address;
    request->readSize = size;

    // send request
    DWORD bytesReturned = 0;
    return DeviceIoControl(
            driverHandle.get(),
            IOCTL_PROC_READ,
            request,
            request->size,
            buffer,
            size,
            &bytesReturned,
            nullptr
    );
}

bool ProcessDriverUtil::write(gameptr_t address, const void *buffer, gamesize_t size) {
    if (!driverHandle.is_valid()) {
        return false;
    }

    // alloc request
    auto mem = std::make_unique<uint8_t[]>(sizeof(WriteMemoryRequest) + size);
    auto request = reinterpret_cast<WriteMemoryRequest *>(mem.get());

    // fill request
    request->size = sizeof(WriteMemoryRequest) + size;
    request->pid = pid;
    request->address = address;
    memcpy(request->data, buffer, size);

    // send request
    DWORD bytesReturned = 0;
    return DeviceIoControl(
            driverHandle.get(),
            IOCTL_PROC_WRITE,
            request,
            request->size,
            nullptr,
            0,
            &bytesReturned,
            nullptr
    );
}


