#ifndef EVIL_CAT_MODULE_H
#define EVIL_CAT_MODULE_H

#include <memory>
#include "module_config.h"

namespace Module {

    extern const std::unique_ptr<ProcessMemoryInterface> processMemory;
    extern const std::unique_ptr<ProcessInfoInterface> processInfo;
    extern const std::unique_ptr<ViewInterface> view;

    extern const std::unique_ptr<GameInterface> game;
    extern const std::unique_ptr<MenuInterface> menu;
};

#endif //EVIL_CAT_MODULE_H
