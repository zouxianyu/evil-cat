#ifndef ASSAULT_CUBE_HACKING_SETTINGS_H
#define ASSAULT_CUBE_HACKING_SETTINGS_H

#include <mutex>
#include <imgui.h>

namespace Settings {

    extern std::mutex mutex;

    extern bool exit;

    extern ImColor textColor;
    extern ImColor backgroundColor;

}

#endif //ASSAULT_CUBE_HACKING_SETTINGS_H
