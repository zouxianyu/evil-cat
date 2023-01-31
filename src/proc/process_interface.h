#ifndef EVIL_CAT_PROC_PROCESS_INTERFACE_H
#define EVIL_CAT_PROC_PROCESS_INTERFACE_H

#include <string>

class ProcessInterface {
public:

    virtual bool attach(uint32_t pid) = 0;

    virtual bool detach() = 0;

    virtual gameptr_t getModuleAddress(const std::string &moduleName) = 0;

    virtual bool read(gameptr_t address, void *buffer, gamesize_t size) = 0;

    virtual bool write(gameptr_t address, const void *buffer, gamesize_t size) = 0;

    virtual ~ProcessInterface() = default;
};

#endif //EVIL_CAT_PROC_PROCESS_INTERFACE_H
