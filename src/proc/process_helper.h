#ifndef EVIL_CAT_PROC_PROCESS_HELPER_H
#define EVIL_CAT_PROC_PROCESS_HELPER_H

#include <vector>
#include <string>

namespace ProcessHelper {
    std::vector<uint32_t> getProcessIdsByName(const std::string& processName);
}

#endif //EVIL_CAT_PROC_PROCESS_HELPER_H
