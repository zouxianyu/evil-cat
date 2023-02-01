#ifndef EVIL_CAT_SETTINGS_H
#define EVIL_CAT_SETTINGS_H

#include <mutex>
#include <imgui.h>

namespace Settings {

    extern std::mutex mutex;

    extern bool showMenu;
    extern bool exit;

    extern ImColor textColor;
    extern ImColor backgroundColor;

    extern float distanceFactor;
}

#endif //EVIL_CAT_SETTINGS_H
