#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose framework module

#include "mem/process_memory_external.h"
#include "proc/process_info_external.h"
#include "view/imgui_d3d9_external/view.h"

using ProcessMemoryImpl = ProcessMemoryExternal;
using ProcessInfoImpl = ProcessInfoExternal;
using ViewImpl = ImGuiD3D9ExternalView;


//////////////////////////////////////////////////////////////////////////
// choose game specific module

#include "game.h"
#include "menu.h"

using GameImpl = Game;
using MenuImpl = Menu;

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H
