#ifndef EVIL_CAT_PROC_PROCESS_INFO_H
#define EVIL_CAT_PROC_PROCESS_INFO_H

#include <optional>
#include <unordered_map>
#include <string>
#include "game_ptr.h"

class ProcessInfoInterface {
public:

    virtual bool attach(const std::string &processName) = 0;

    virtual bool detach() = 0;

    virtual std::optional<gameptr_t> getModuleAddress(const std::string &moduleName) = 0;

    virtual bool refresh() = 0;

    virtual ~ProcessInfoInterface() = default;
};

#endif //EVIL_CAT_PROC_PROCESS_INFO_H
