#ifndef EVIL_CAT_PROC_PROCESS_HELPER_H
#define EVIL_CAT_PROC_PROCESS_HELPER_H

#include <vector>
#include <string>

namespace ProcessHelper {
    std::vector<uint32_t> getProcessIdsByName(const std::string &processName);

    bool isAlive(uint32_t pid);

    uint32_t getProcessIdByName(const std::string &processName, size_t index = 0);
}

#endif //EVIL_CAT_PROC_PROCESS_HELPER_H
