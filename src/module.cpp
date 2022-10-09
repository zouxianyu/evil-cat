#include <memory>
#include "module_config.h"
#include "module.h"

namespace Module {

    const std::unique_ptr<ProcessMemoryInterface> processMemory =
            std::make_unique<ProcessMemoryImpl>();

    const std::unique_ptr<ProcessInfoInterface> processInfo =
            std::make_unique<ProcessInfoImpl>();

    const std::unique_ptr<ViewInterface> view = std::make_unique<ViewImpl>();

    const std::unique_ptr<GameInterface> game = std::make_unique<GameImpl>();

    const std::unique_ptr<MenuInterface> menu = std::make_unique<MenuImpl>();

}