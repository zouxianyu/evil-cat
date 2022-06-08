#include "process_memory.h"
#include "process_memory_internal.h"
#include "process_memory_external.h"

ProcessMemory &ProcessMemory::getInstance() {
#if (CONF_INTERNAL == 1)
    return ProcessMemoryInternal::getInstance();
#else
    return ProcessMemoryExternal::getInstance();
#endif
}
