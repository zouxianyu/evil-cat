#ifndef EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H
#define EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H

#include <optional>
#include <windows.h>
#include "game_ptr.h"
#include "process_info_interface.h"

class ProcessInfoExternal : public ProcessInfoInterface {

    HANDLE hProcess = nullptr;

    std::unordered_map<std::string, gameptr_t> modulesAddress;

    DWORD getProcessIdByName(const std::string& processName);

public:
    bool attach(const std::string &processName) override;

    bool detach() override;

    std::optional<gameptr_t> getModuleAddress(const std::string &moduleName) override;

    bool refresh() override;
};

#endif //EVIL_CAT_PROC_PROCESS_INFO_EXTERNAL_H
