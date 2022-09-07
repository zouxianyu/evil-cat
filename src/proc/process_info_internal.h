#ifndef EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H
#define EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H

#include "process_info.h"

class ProcessInfoInternal : public ProcessInfo {

    std::unordered_map<std::string, void*> modulesAddress;

public:
    static ProcessInfo &getInstance();

    bool attach(const std::string &processName) override;

    bool detach() override;

    bool getModuleAddress(const std::string &moduleName, void *&address) override;

    bool refresh() override;
};

#endif //EVIL_CAT_PROC_PROCESS_INFO_INTERNAL_H
