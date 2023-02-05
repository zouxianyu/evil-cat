#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose framework module

#include "proc/process_external.h"
#include "view/imgui_d3d9_external/view.h"

using ProcessImpl = ProcessExternal;
using ViewImpl = ImGuiD3D9ExternalView;


//////////////////////////////////////////////////////////////////////////
// choose game specific module

#include "game.h"

using GameImpl = Game;

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_MODULE_CONFIG_H
