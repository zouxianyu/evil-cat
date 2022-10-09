#ifndef EVIL_CAT_MEM_PROCESS_MEMORY_EXTERNAL_H
#define EVIL_CAT_MEM_PROCESS_MEMORY_EXTERNAL_H

#include <windows.h>
#include "process_memory_interface.h"

class ProcessMemoryExternal : public ProcessMemoryInterface {

    HANDLE hProcess = nullptr;

    DWORD getProcessIdByName(const std::string& processName);

public:
    bool attach(const std::string &processName) override;

    bool detach() override;

    bool read(gameptr_t address, void *buffer, size_t size) override;

    bool write(gameptr_t address, const void *buffer, size_t size) override;

};

#endif //EVIL_CAT_MEM_PROCESS_MEMORY_EXTERNAL_H
