#ifndef EVIL_CAT_PROC_PROCESS_INTERNAL_H
#define EVIL_CAT_PROC_PROCESS_INTERNAL_H

#include <optional>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "process_interface.h"

class ProcessInternal : public ProcessInterface {

public:
    bool attach(uint32_t pid) override;

    bool detach() override;

    gameptr_t getModuleAddress(const std::string &moduleName) override;

    bool read(gameptr_t address, void *buffer, gamesize_t size) override;

    bool write(gameptr_t address, const void *buffer, gamesize_t size) override;
};

#endif //EVIL_CAT_PROC_PROCESS_INTERNAL_H
