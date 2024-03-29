#ifndef EVIL_CAT_MODULE_H
#define EVIL_CAT_MODULE_H

#include <memory>
#include "module_config.h"

namespace Module {
    extern const std::unique_ptr<ProcessInterface> process;

    extern const std::unique_ptr<ViewInterface> view;

    extern const std::unique_ptr<GameInterface> game;
}

#endif //EVIL_CAT_MODULE_H
