#ifndef EVIL_CAT_PROC_PROCESS_EXTERNAL_H
#define EVIL_CAT_PROC_PROCESS_EXTERNAL_H

#include <windows.h>
#include "process_interface.h"

class ProcessExternal : public ProcessInterface {

    HANDLE hProcess = nullptr;

public:
    bool attach(uint32_t pid) override;

    bool detach() override;

    gameptr_t getModuleAddress(const std::string &moduleName) override;

    bool read(gameptr_t address, void *buffer, gamesize_t size) override;

    bool write(gameptr_t address, const void *buffer, gamesize_t size) override;
};

#endif //EVIL_CAT_PROC_PROCESS_EXTERNAL_H
