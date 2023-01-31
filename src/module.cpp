#include <memory>
#include "cache/game_cache.h"
#include "module_config.h"
#include "module.h"

namespace Module {
    const std::unique_ptr<ProcessInterface> process = std::make_unique<ProcessImpl>();

    const std::unique_ptr<ViewInterface> view = std::make_unique<ViewImpl>();

    const std::unique_ptr<GameInterface> game = std::make_unique<GameCache>(
            std::move(std::make_unique<GameImpl>())
    );
}