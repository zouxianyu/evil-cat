#ifndef ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_H
#define ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_H

#include <string>

class ProcessMemory {
public:

    static ProcessMemory &getInstance();

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

    virtual bool read(void *address, void *buffer, size_t size) = 0;

    virtual bool write(void *address, const void *buffer, size_t size) = 0;

};

#endif //ASSAULT_CUBE_HACKING_MEM_PROCESS_MEMORY_H
