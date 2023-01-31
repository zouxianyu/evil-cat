#ifndef EVIL_CAT_PROC_PROCESS_DRIVER_UTIL_H
#define EVIL_CAT_PROC_PROCESS_DRIVER_UTIL_H

#include <windows.h>
#include "driver_loader/simple_driver_loader.h"
#include "smart_handle.h"
#include "process_interface.h"

#define DEVICE_NAME         "\\\\.\\MemRWDrv"
#define DRIVER_PATH         "kernel_util.sys"
#define DISPLAY_NAME        "KernelUtil"
#define IOCTL_PROC_READ     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROC_WRITE    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROC_MODULE   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

class ProcessDriverUtil : public ProcessInterface {

    using DriverLoaderImpl = SimpleDriverLoader;

    std::unique_ptr<DriverLoaderInterface> driverLoader = std::make_unique<DriverLoaderImpl>();
    SmartHandle driverHandle;
    uint32_t pid = 0;

    bool openDriver();

    void closeDriver();

public:
    bool attach(uint32_t pid) override;

    bool detach() override;

    gameptr_t getModuleAddress(const std::string &moduleName) override;

    bool read(gameptr_t address, void *buffer, gamesize_t size) override;

    bool write(gameptr_t address, const void *buffer, gamesize_t size) override;
};

struct RequestBase {
    uint32_t size;
};

struct GetModuleAddressRequest : RequestBase {
    uint32_t pid;
    char moduleName[0];
};

struct ReadMemoryRequest : RequestBase {
    uint32_t pid;
    uint64_t address;
    uint64_t readSize;
};

struct WriteMemoryRequest : RequestBase {
    uint32_t pid;
    uint64_t address;
    uint8_t data[0];
};

#endif //EVIL_CAT_PROC_PROCESS_DRIVER_UTIL_H
