#ifndef EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H
#define EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H

#include <unordered_map>
#include "process_memory.h"

class ProcessMemoryInternal : ProcessMemory {

public:
    static ProcessMemory &getInstance();

    bool attach(const std::string &processName) override;

    bool detach() override;

    bool read(void *address, void *buffer, size_t size) override;

    bool write(void *address, const void *buffer, size_t size) override;

};

#endif //EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H
