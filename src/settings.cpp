#include <mutex>
#include <imgui.h>
#include "settings.h"

namespace Settings {

    // must hold the mutex before using all the fields below
    std::mutex mutex;

    bool showMenu = true;
    bool exit = false;

    ImColor textColor = ImColor(255, 255, 255);
    ImColor backgroundColor = ImColor(0, 0, 0);

}
