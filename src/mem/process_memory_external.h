#ifndef ASSAULT_CUBE_HACKING_PROCESS_MEM_MEMORY_EXTERNAL_H
#define ASSAULT_CUBE_HACKING_PROCESS_MEM_MEMORY_EXTERNAL_H

#include <windows.h>
#include "process_memory.h"

class ProcessMemoryExternal : ProcessMemory {

    HANDLE hProcess = nullptr;

    DWORD getProcessIdByName(const std::string& processName);

public:
    static ProcessMemory &getInstance();

    bool attach(const std::string &processName) override;

    bool detach() override;

    bool read(void *address, void *buffer, size_t size) override;

    bool write(void *address, const void *buffer, size_t size) override;

};

#endif //ASSAULT_CUBE_HACKING_PROCESS_MEM_MEMORY_EXTERNAL_H
