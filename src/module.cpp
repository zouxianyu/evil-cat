#include <memory>
#include "module_config.h"
#include "module.h"

namespace Module {

    const std::unique_ptr<ProcessMemoryInterface> processMemory = std::make_unique<ProcessMemory>();
    const std::unique_ptr<ProcessInfoInterface> processInfo = std::make_unique<ProcessInfo>();
    const std::unique_ptr<ViewInterface> view = std::make_unique<View>();

}