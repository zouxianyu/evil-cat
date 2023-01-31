#ifndef EVIL_CAT_GAME_PUBG_MODULE_CONFIG_H
#define EVIL_CAT_GAME_PUBG_MODULE_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// choose framework module

#include "proc/process_driver_util.h"
#include "view/imgui_dwm/view.h"

using ProcessImpl = ProcessDriverUtil;
using ViewImpl = ImGuiDWMView;


//////////////////////////////////////////////////////////////////////////
// choose game specific module

#include "game.h"

using GameImpl = Game;

#endif //EVIL_CAT_GAME_PUBG_MODULE_CONFIG_H
