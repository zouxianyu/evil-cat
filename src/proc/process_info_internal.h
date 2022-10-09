#ifndef EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H
#define EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H

#include <optional>
#include <unordered_map>
#include <string>
#include "process_info_interface.h"

class ProcessInfoInternal : public ProcessInfoInterface {

    std::unordered_map<std::string, gameptr_t> modulesAddress;

public:
    bool attach(const std::string &processName) override;

    bool detach() override;

    std::optional<gameptr_t> getModuleAddress(const std::string &moduleName) override;

    bool refresh() override;
};

#endif //EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H
