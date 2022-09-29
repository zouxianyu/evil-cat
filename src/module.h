#ifndef EVIL_CAT_MODULE_H
#define EVIL_CAT_MODULE_H

#include <memory>
#include "module_config.h"

namespace Module {

    extern const std::unique_ptr<ProcessMemoryInterface> processMemory;
    extern const std::unique_ptr<ProcessInfoInterface> processInfo;
    extern const std::unique_ptr<ViewInterface> view;

};

#endif //EVIL_CAT_MODULE_H
