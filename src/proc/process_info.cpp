#include "process_info.h"
#include "process_info_internal.h"
#include "process_info_external.h"

ProcessInfo &ProcessInfo::getInstance() {
#if (CONF_INTERNAL == 1)
    return ProcessInfoInternal::getInstance();
#else
    return ProcessInfoExternal::getInstance();
#endif
}
