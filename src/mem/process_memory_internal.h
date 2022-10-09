#ifndef EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H
#define EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H

#include <unordered_map>
#include "process_memory_interface.h"

class ProcessMemoryInternal : public ProcessMemoryInterface {

public:
    bool attach(const std::string &processName) override;

    bool detach() override;

    bool read(gameptr_t address, void *buffer, gamesize_t size) override;

    bool write(gameptr_t address, const void *buffer, gamesize_t size) override;

};

#endif //EVIL_CAT_MEM_PROCESS_MEMORY_INTERNAL_H
