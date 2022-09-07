#ifndef EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H
#define EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H

#include <windows.h>
#include "process_info.h"

class ProcessInfoExternal : public ProcessInfo {
    HANDLE hProcess = nullptr;
    std::unordered_map<std::string, void*> modulesAddress;
    DWORD getProcessIdByName(const std::string& processName);
public:
    static ProcessInfo &getInstance();

    bool attach(const std::string &processName) override;

    bool detach() override;

    bool getModuleAddress(const std::string &moduleName, void *&address) override;

    bool refresh() override;
};

#endif //EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H
