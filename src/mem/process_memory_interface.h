#ifndef EVIL_CAT_MEM_PROCESS_MEMORY_INTERFACE_H
#define EVIL_CAT_MEM_PROCESS_MEMORY_INTERFACE_H

#include <string>

class ProcessMemoryInterface {
public:

    /*
     * attach to a specific process
     * nothing to do in internal mode
     */
    virtual bool attach(const std::string &processName) = 0;

    /*
     * detach from the process
     * nothing to do in external mode
     */
    virtual bool detach() = 0;

    virtual bool read(gameptr_t address, void *buffer, size_t size) = 0;

    virtual bool write(gameptr_t address, const void *buffer, size_t size) = 0;

    virtual ~ProcessMemoryInterface() = default;
};

#endif //EVIL_CAT_MEM_PROCESS_MEMORY_INTERFACE_H
