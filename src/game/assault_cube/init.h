#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_INIT_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_INIT_H

#include <memory>
#include "service/service_interface.h"

std::vector<std::unique_ptr<ServiceInterface>> init();

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_INIT_H
